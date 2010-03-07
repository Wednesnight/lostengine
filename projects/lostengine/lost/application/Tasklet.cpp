#include "lost/lua/State.h"
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
#include "lost/application/Window.h"
#include "lost/event/EventDispatcher.h"

using namespace boost;
using namespace luabind;
using namespace std;
using namespace lost::event;
using namespace lost::lua;
using namespace lost::platform;
using namespace lost::resource;
using namespace lost::rg;

namespace lost
{
  namespace application
  {
    
    Tasklet::Tasklet(LoaderPtr inLoader)
    {
      isAlive = false;
      waitForEvents = false;
      name = "<unnamed tasklet>";
	    window = NULL;

      loader = inLoader;
      eventDispatcher.reset(new event::EventDispatcher());
      lua.reset(new State(loader));
      renderNode.reset(new Node());
      updateQueue.reset(new Queue());

      // bind lostengine lua mappings
      bindAll(*lua);
      // install custom module loader so require goes through resourceLoader
      ModuleLoader::install(*lua);
      // publish global utility functions
      GlobalFunctions::install(*lua);
    }
    
    Tasklet::~Tasklet()
    {
      // first: clear the dispatcher/callbacks and cleanup all lua callback resources
      eventDispatcher->clear();
      luabind::object nil;
      luaStartup = nil;
      luaUpdate = nil;
      luaShutdown = nil;
      renderNode.reset();
      updateQueue.reset();
      if (window) delete window;
      loader.reset(); // loader is also present in lua state, so kill it first
      eventDispatcher.reset(); 
      lua.reset();
    }

    void Tasklet::init()
    {  
      // populate self into lua context
      lua->globals["lost"]["application"]["currentTasklet"] = this;

      // try to load the main script and memorize result in a flag
      scriptLoaded = false;
      try
      {
        lua->doFile("main.lua");
        scriptLoaded = true;
      }
      catch(resource::LoaderError& ex)
      {
        DOUT("couldn't load script 'main.lua', reason: "+ string(ex.what()));
      }

      // try to extract window params and flag from interpreter
      // the values are optional and set as globals
      luabind::object obj = lua->globals["windowParams"];
      if(obj)
      {
        createWindow(object_cast<WindowParams>(obj));
      }
            
      // get lua functions if they are present
      hasLuaStartup = false;
      hasLuaUpdate = false;
      hasLuaShutdown = false;
      luaStartup = lua->globals["startup"];
      if(luabind::type(luaStartup)==LUA_TFUNCTION) hasLuaStartup=true; else DOUT("no startup() found in Lua");
      luaUpdate = lua->globals["update"];
      if(luabind::type(luaUpdate)==LUA_TFUNCTION) hasLuaUpdate=true; else DOUT("no update() found in Lua");
      luaShutdown = lua->globals["shutdown"];
      if(luabind::type(luaShutdown)==LUA_TFUNCTION) hasLuaShutdown=true; else DOUT("no shutdown() found in Lua");
    }

    void Tasklet::render()
    {
      // render
      if(window != NULL)
      {
        renderNode->process(window->context);
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
      bool result = true;
      // make sure that our GL context is the current context on startup of this thread
      if(window != NULL)
      {
        window->context->makeCurrent();
      }
      if(hasLuaStartup)
      {
        result = call_function<bool>(luaStartup, this);
      }
      updateQueue->process(this);      
      return result;
    }

    bool Tasklet::update()
    {
      bool result = true;
      if(hasLuaUpdate)
      {
        result = call_function<bool>(luaUpdate, this);
      }
      updateQueue->process(this);      
      return  result;      
    }

    bool Tasklet::shutdown()
    {
      bool result = true;
      if(hasLuaShutdown)
      {
        result = call_function<bool>(luaShutdown, this);
      }
      return  result;      
    }
    
    void Tasklet::createWindow(const WindowParams& params)
    {
      window = new Window(eventDispatcher, params);
      window->dispatcher->addEventListener(WindowEvent::CLOSE(), event::receive<WindowEvent>(bind(&Tasklet::closeWindow, this, _1)));
      window->open();        
    }

    void Tasklet::closeWindow(WindowEventPtr event)
    {
      stop();
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
