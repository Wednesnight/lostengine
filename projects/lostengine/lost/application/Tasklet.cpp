#include "lost/lua/lua.h"
#include "lost/lua/State.h"
#include "lost/gl/Context.h"
#include "lost/event/Event.h"
#include "lost/event/EventDispatcher.h"
#include "lost/application/Tasklet.h"
#include "lost/application/Application.h"
#include <boost/bind.hpp>
#include "lost/lua/BindAll.h"
#include "lost/lua/ModuleLoader.h"
#include "lost/lua/GlobalFunctions.h"
#include <algorithm>
#include "lost/platform/Platform.h"
#include "lost/common/Logger.h"
#include "lost/application/TaskletEvent.h"
#include "lost/application/Queue.h"
#include "lost/application/QueueEvent.h"
#include "lost/application/ProcessEvent.h"
#include "lost/application/WindowEvent.h"
#include "lost/event/Receive.h"
#include "lost/rg/Node.h"
#include "lost/rg/Clear.h"
#include "lost/rg/ClearColor.h"
#include "lost/application/Window.h"
#include "lost/event/EventDispatcher.h"
#include "lost/gl/gl.h"
#include <boost/filesystem.hpp>
#include "lost/font/FontManager.h"
#include "lost/application/ResizeEvent.h"
#include "lost/application/KeyEvent.h"

using namespace boost;
using namespace luabind;
using namespace std;
using namespace lost::event;
using namespace lost::lua;
using namespace lost::platform;
using namespace lost::resource;
using namespace lost::rg;
using namespace lost::common;
using namespace lost::font;

namespace lost
{
  namespace application
  {
   
    struct Tasklet::LuaStateHelper
    {
      luabind::object luaStartup;
      luabind::object luaUpdate;
      luabind::object luaShutdown;
      luabind::object luaKey;
      luabind::object config;
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
      // publish global utility functions
      GlobalFunctions::install(*lua);
      lsh.reset(new LuaStateHelper);
      eventDispatcher->addEventListener(ResizeEvent::TASKLET_WINDOW_RESIZE(), event::receive<ResizeEvent>(boost::bind(&Tasklet::updateWindowSize, this, _1)));      
      eventDispatcher->addEventListener(KeyEvent::KEY_UP(), event::receive<KeyEvent>(boost::bind(&Tasklet::key, this, _1)));      
      eventDispatcher->addEventListener(KeyEvent::KEY_DOWN(), event::receive<KeyEvent>(boost::bind(&Tasklet::key, this, _1)));      
    }
    
    Tasklet::~Tasklet()
    {
    }

    void Tasklet::init()
    {  
      // populate self into lua context
      lua->globals["tasklet"] = this;

      try
      {
        lua->doFile("lost/common/FontInit.lua");
      }
      catch(std::exception& ex)
      {
        WOUT("couldn't initialize fonts, reason: "+ string(ex.what()));
      }

      configLoaded = config.load(lua, loader);
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
      if(luabind::type(lsh->luaStartup)==LUA_TFUNCTION) hasLuaStartup=true; //else DOUT("no startup() found in Lua");
      lsh->luaUpdate = lua->globals["update"];
      if(luabind::type(lsh->luaUpdate)==LUA_TFUNCTION) hasLuaUpdate=true; //else DOUT("no update() found in Lua");
      lsh->luaShutdown = lua->globals["shutdown"];
      if(luabind::type(lsh->luaShutdown)==LUA_TFUNCTION) hasLuaShutdown=true; //else DOUT("no shutdown() found in Lua");
      lsh->luaKey = lua->globals["key"];
      if(luabind::type(lsh->luaKey)==LUA_TFUNCTION) hasLuaKey=true; //else DOUT("no key() found in Lua");
    }

    void Tasklet::cleanup()
    {
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

    void Tasklet::render()
    {
      // render
      if(window != NULL)
      {
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
      if(hasLuaStartup)
      {
        call_function<void>(lsh->luaStartup);
      }
      updateQueue->process(this);      
      return running;
    }

    bool Tasklet::update(double deltaSeconds)
    {
      if(hasLuaUpdate)
      {
        call_function<void>(lsh->luaUpdate, deltaSeconds);
      }
//-- disableed so the queue is only cleared once per frame      updateQueue->process(this);      
      return  running;      
    }

    void Tasklet::shutdown()
    {
      if(hasLuaShutdown)
      {
        call_function<void>(lsh->luaShutdown);
      }
    }
    
    void Tasklet::key(const application::KeyEventPtr& ev)
    {
      if(hasLuaKey)
      {
        call_function<void>(lsh->luaKey,ev);
      }
    }
    
    void Tasklet::createWindow()
    {
      window = new Window(eventDispatcher, &config);
      window->dispatcher->addEventListener(WindowEvent::CLOSE(), event::receive<WindowEvent>(boost::bind(&Tasklet::closeWindow, this, _1)));
      window->open();        
    }

    void Tasklet::closeWindow(WindowEventPtr event)
    {
      dispatchApplicationEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::TERMINATE(), this)));
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
  }
}
