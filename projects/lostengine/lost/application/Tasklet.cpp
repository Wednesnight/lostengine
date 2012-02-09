/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/lua/lua.h"
#include "lost/lua/State.h"
#include "lost/gl/Context.h"
#include "lost/event/Event.h"
#include "lost/event/EventDispatcher.h"
#include "lost/application/Tasklet.h"
#include "lost/application/Application.h"
#include "lost/lua/BindAll.h"
#include "lost/lua/ModuleLoader.h"
#include "lost/common/Logger.h"
#include "lost/application/TaskletEvent.h"
#include "lost/application/Queue.h"
#include "lost/application/QueueEvent.h"
#include "lost/application/ProcessEvent.h"
#include "lost/application/WindowEvent.h"
#include "lost/application/DebugEvent.h"
#include "lost/rg/Node.h"
#include "lost/rg/Clear.h"
#include "lost/rg/ClearColor.h"
#include "lost/application/Window.h"
#include "lost/event/EventDispatcher.h"
#include "lost/gl/gl.h"
#include "lost/font/FontManager.h"
#include "lost/application/ResizeEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/time/Clock.h"
#include "lost/time/ThreadedTimerScheduler.h"
#include "lost/profiler/Blackbox.h"
#include "lost/event/Listener.h"
#include "lost/resource/FilesystemRepository.h"
#include "lost/resource/ApplicationResourceRepository.h"
#include "lost/application/SpawnTaskletEvent.h"

using namespace lost::event;
using namespace lost::lua;
using namespace lost::platform;
using namespace lost::resource;
using namespace lost::rg;
using namespace lost::common;
using namespace lost::font;
using namespace lost::time;

namespace lost
{
  namespace application
  {
   
    struct Tasklet::LuaStateHelper
    {
      slub::reference luaStartup;
      slub::reference luaUpdate;
      slub::reference luaShutdown;
      slub::reference luaKey;
      slub::reference config;
    };

    void Tasklet::updateWindowSize(const application::ResizeEventPtr& event)
    {
      window->size = math::Vec2(event->width, event->height);
    }
      
    Tasklet::Tasklet(LoaderPtr inLoader)
    {
      isAlive = false;
      waitForEvents = false;
      name = "<unnamed tasklet>";
	    window = NULL;
      fontManager = FontManager::create(inLoader);
      running = true;
      scriptLoaded = false;
      configLoaded = false;
      loader = inLoader;
      eventDispatcher.reset(new event::EventDispatcher());
      lua.reset(new State(loader));
      clearNode.reset(new Node());
      clearNode->add(NodePtr(new ClearColor(Color(1,1,0))));
      clearNode->add(NodePtr(new Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)));
      renderNode.reset(new Node());
      uiNode.reset(new Node());
      updateQueue.reset(new Queue());

      // bind lostengine lua mappings
      bindAll(*lua);
      // install custom module loader so require goes through resourceLoader
      ModuleLoader::install(*lua);
      lsh.reset(new LuaStateHelper);
      eventDispatcher->addEventListener(ResizeEvent::TASKLET_WINDOW_RESIZE(), event::makeListener(this, &Tasklet::updateWindowSize));      
      eventDispatcher->addEventListener(KeyEvent::KEY_UP(), event::makeListener(this, &Tasklet::key));      
      eventDispatcher->addEventListener(KeyEvent::KEY_DOWN(), event::makeListener(this, &Tasklet::key));      

      eventDispatcher->addEventListener(DebugEvent::CMD_MEM_INFO(), event::makeListener(this, &Tasklet::handleDebugEvent));
      eventDispatcher->addEventListener(DebugEvent::CMD_PAUSE(), event::makeListener(this, &Tasklet::handleDebugEvent));
      eventDispatcher->addEventListener(DebugEvent::CMD_CONTINUE(), event::makeListener(this, &Tasklet::handleDebugEvent));
    }
    
    Tasklet::~Tasklet()
    {
    }

    void Tasklet::start()
    {  
      // populate self into lua context
      lua->globals["tasklet"] = this;

      try
      {
        lua->doFile("lost/TaskletInit.lua");
      }
      catch(std::exception& ex)
      {
        WOUT("couldn't initialize fonts, reason: "+ string(ex.what()));
      }
      
      try {
        configLoaded = config.load(lua, loader);
      }
      catch (std::exception& ex) {
        WOUT("couldn't load script 'config.lua', reason: "+ string(ex.what()));
      }

      name = config.taskletName;
      waitForEvents = config.taskletWaitForEvents;
      // try to load the main script and memorize result in a flag
      try
      {
        lua->doFile("main.lua");
        scriptLoaded = true;
      }
      catch(resource::LoaderError& ex)
      {
        DOUT("couldn't load script 'main.lua', reason: "+ string(ex.what()));
      }
      if(config.taskletHasWindow)
      {
        createWindow();
      }
            
      // get lua functions if they are present
      hasLuaStartup = false;
      hasLuaUpdate = false;
      hasLuaShutdown = false;
      hasLuaKey = false;
      lsh->luaStartup = lua->globals["startup"];
      if(lsh->luaStartup.type()==LUA_TFUNCTION) hasLuaStartup=true; //else DOUT("no startup() found in Lua");
      lsh->luaUpdate = lua->globals["update"];
      if(lsh->luaUpdate.type()==LUA_TFUNCTION) hasLuaUpdate=true; //else DOUT("no update() found in Lua");
      lsh->luaShutdown = lua->globals["shutdown"];
      if(lsh->luaShutdown.type()==LUA_TFUNCTION) hasLuaShutdown=true; //else DOUT("no shutdown() found in Lua");
      lsh->luaKey = lua->globals["key"];
      if(lsh->luaKey.type()==LUA_TFUNCTION) hasLuaKey=true; //else DOUT("no key() found in Lua");

      thread.reset(new TaskletThread(this));
      thread->start();
    }

    void Tasklet::stop()
    {
      if (isAlive)
      {
        thread->stop();
        // wakeup
        if (waitForEvents) {
          eventDispatcher->wakeup();
        }
        thread->wait();
      }
      else
      {
        // make sure that our GL context is the current context
        if(window != NULL)
        {
          window->context->makeCurrent();
        }
        // first: clear the dispatcher/callbacks and cleanup all lua callback resources
        eventDispatcher->clear();
        fontManager.reset();
        lsh.reset();
        clearNode.reset();
        renderNode.reset();
        uiNode.reset();
        updateQueue.reset();
        loader.reset(); // loader is also present in lua state, so kill it first
        eventDispatcher.reset(); 
        if (window) window->context->cleanup();
        lua.reset();
        if (window) delete window;
      }
    }

    void Tasklet::render()
    {
      // render
      if(window != NULL)
      {
        window->context->makeCurrent();
        clearNode->process(window->context);
        renderNode->process(window->context);
        uiNode->process(window->context);
        window->context->swapBuffers();
      }    
    }
    
    void Tasklet::processEvents()
    {
      eventDispatcher->processEvents();
      updateQueue->process(this);      
    }

    bool Tasklet::startup()
    {    
      scheduler.reset(new ThreadedTimerScheduler(name, eventDispatcher));    
      if(hasLuaStartup)
      {
        lsh->luaStartup();
      }
      updateQueue->process(this);     
      BB_SET_CLEAR("Lua Memory", true);
      BB_SET_UNIT("Lua Memory","kb"); 
      return running;
    }

    bool Tasklet::update(double deltaSeconds)
    {
      if(hasLuaUpdate)
      {
        lsh->luaUpdate(deltaSeconds);
      }
      BB_SET("Lua Memory",lua->memUsage());
//      BB_LOG;
      BB_SNAP;
      return  running;      
    }

    void Tasklet::shutdown()
    {
      if(hasLuaShutdown)
      {
        lsh->luaShutdown();
      }
    }
    
    void Tasklet::key(const application::KeyEventPtr& ev)
    {
      if(hasLuaKey)
      {
        lsh->luaKey(ev);
      }
    }
    
    void Tasklet::createWindow()
    {
      window = new Window(eventDispatcher, &config);
      window->dispatcher->addEventListener(WindowEvent::CLOSE(), event::makeListener(this, &Tasklet::closeWindow));
      window->open();        
    }

    void Tasklet::closeWindow(const WindowEventPtr& event)
    {
      dispatchApplicationEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::TERMINATE(), this)));
    }
    
    void Tasklet::spawnTasklet(const string& taskletPath)
    {
      LoaderPtr loader = Loader::create();
      loader->addRepository(resource::FilesystemRepository::create(taskletPath));
      loader->addRepository(ApplicationResourceRepository::create());
      SpawnTaskletEventPtr ev = SpawnTaskletEvent::create(loader);
      dispatchApplicationEvent(ev); // sends the event to the application on the main thread
    }

    
    void Tasklet::queueApplicationEvent(EventPtr event)
    {
      QueueEventPtr queue(new QueueEvent(event));
      eventDispatcher->dispatchEvent(queue);
    }

    void Tasklet::dispatchApplicationEvent(EventPtr event)
    {
      queueApplicationEvent(event);
      processApplicationEvents();
    }

    void Tasklet::processApplicationEvents()
    {
      ProcessEventPtr process(new ProcessEvent());
      eventDispatcher->dispatchEvent(process);
    }
    
    bool Tasklet::alive()
    {
      return isAlive;
    }

    void TaskletLuaDebugHook(lua_State* state, lua_Debug* debug)
    {
      slub::reference taskletObj = slub::globals(state)["tasklet"];
      if (taskletObj.type() != LUA_TNIL) {
        Tasklet* tasklet = taskletObj.cast<Tasklet*>();
        DebugEventPtr event = DebugEvent::create(DebugEvent::PAUSE(), tasklet);
        lua_getinfo(state, "Slnu", debug);
        event->info.debug = debug;
        tasklet->eventDispatcher->dispatchEvent(event);
        tasklet->eventDispatcher->waitForEvent(DebugEvent::CMD_CONTINUE());
      }
    }

    void Tasklet::handleDebugEvent(const DebugEventPtr& event)
    {
      if (event->tasklet == this) {

        if (event->type == DebugEvent::CMD_MEM_INFO()) {
          DebugEventPtr result = DebugEvent::create(DebugEvent::MEM_INFO(), this);
          result->info.memSize = lua->memUsage();
          eventDispatcher->dispatchEvent(result);
        }

        else if (event->type == DebugEvent::CMD_PAUSE()) {
          lua_sethook(lua->state, &TaskletLuaDebugHook, LUA_MASKCALL | LUA_MASKRET | LUA_MASKLINE, 0);
        }

        else if (event->type == DebugEvent::CMD_CONTINUE() && event->mode == 0) {
          lua_sethook(lua->state, NULL, 0, 0);
        }
      }
    }

  }
}
