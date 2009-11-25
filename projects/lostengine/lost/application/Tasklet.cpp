#include "lost/lua/State.h"
#include "lost/event/EventDispatcher.h"
#include "lost/application/Tasklet.h"
#include "lost/application/Application.h"
#include <boost/bind.hpp>
#include "lost/lua/BindAll.h"
#include "lost/lua/ModuleLoader.h"
#include <algorithm>
#include "lost/platform/Platform.h"
#include "lost/common/Logger.h"
#include "lost/application/mac/ThreadAutoreleasePoolHack.h"
#include "lost/application/TaskletEvent.h"
#include "lost/application/QueueEvent.h"
#include "lost/application/ProcessEvent.h"
#include "lost/application/WindowEvent.h"
#include "lost/event/Receive.h"

using namespace boost;
using namespace fhtagn::threads;
using namespace luabind;
using namespace std;
using namespace lost::event;
using namespace lost::lua;
using namespace lost::platform;
using namespace lost::resource;

namespace lost
{
  namespace application
  {
    
    Tasklet::Tasklet(LoaderPtr inLoader)
    : tasklet::tasklet(bind(&Tasklet::run, this, _1)),
      loader(inLoader),
      eventDispatcher(new EventDispatcher()),
      lua(new State(loader)),
      waitForEvents(false),
      scriptname("main.lua")
    {
      // set error handler
      add_error_handler(bind(&Tasklet::error, this, _1, _2));

      // bind lostengine lua mappings
      bindAll(*lua);
      // install custom module loader so require goes through resourceLoader
      ModuleLoader::install(*lua, loader);
    }
    
    Tasklet::~Tasklet()
    {
      // first: clear the dispatcher/callbacks and cleanup all lua callback resources
      eventDispatcher->clear();
      luabind::object nil;
      luaStartup = nil;
      luaUpdate = nil;
      luaShutdown = nil;
      luaProcessCallLater = nil;
      // second: reset lua state to cleanup all references back into native code
      lua.reset();
      // third: cleanup remaining resources
      if (window) delete window;
    }

    void Tasklet::init()
    {      
      // try to load the main script and memorize result in a flag
      scriptLoaded = false;
      try
      {
        lua->doFile(scriptname);
        scriptLoaded = true;
      }
      catch(resource::LoaderError& ex)
      {
        DOUT("couldn't load script <"+ scriptname +">: "+ string(ex.what()));
      }

      // try to extract window params and flag from interpreter
      // the values are optional and set as globals
      luabind::object obj = lua->globals["windowParams"];
      if(obj)
      {
        windowParams=object_cast<WindowParams>(obj);
        window = new Window(eventDispatcher, windowParams);
        window->dispatcher->addEventListener(WindowEvent::CLOSE(), event::receive<WindowEvent>(bind(&Tasklet::closeWindow, this, _1)));
        window->open();        
      }
            
      // get lua functions if they are present
      hasLuaStartup = false;
      hasLuaUpdate = false;
      hasLuaShutdown = false;
      hasLuaProcessCallLater = false;
      luaStartup = lua->globals["startup"];
      if(luabind::type(luaStartup)==LUA_TFUNCTION) hasLuaStartup=true; else DOUT("no startup() found in Lua");
      luaUpdate = lua->globals["update"];
      if(luabind::type(luaUpdate)==LUA_TFUNCTION) hasLuaUpdate=true; else DOUT("no update() found in Lua");
      luaShutdown = lua->globals["shutdown"];
      if(luabind::type(luaShutdown)==LUA_TFUNCTION) hasLuaShutdown=true; else DOUT("no shutdown() found in Lua");
      luaProcessCallLater = lua->globals["processCallLaterQueue"];
      if(luabind::type(luaProcessCallLater)==LUA_TFUNCTION) hasLuaProcessCallLater=true; else DOUT("no processCallLaterQueue() found in Lua");
    }

    void Tasklet::run(tasklet& tasklet)
    {
      void* pool = threadAutoreleasePoolHack_createPool();

      // make sure that our GL context is the current context
      if(window)
      {
        window->context->makeCurrent();      
      }
        
      if (startup())
      {
        while (get_state() == RUNNING && update())
        {
          const double startTime = currentTimeMilliSeconds();
          if(!waitForEvents)
          {
            eventDispatcher->processEvents(fmax(0, 1.0/60.0 - currentTimeMilliSeconds() - startTime));
          }
          else
          {
            eventDispatcher->waitForEvents();
            // no event processing timeout since we probably don't have that many events
            eventDispatcher->processEvents(0);
          }
          pool = threadAutoreleasePoolHack_drainAndRecreatePool(pool);
        }
        shutdown();
      }
      threadAutoreleasePoolHack_drainPool(pool);        
      dispatchApplicationEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), this)));
    }

    void Tasklet::error(fhtagn::threads::tasklet& tasklet, std::exception const& exception)
    {
      EOUT(exception.what());
      dispatchApplicationEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), this)));
    }

    bool Tasklet::startup()
    {    
      bool result = true;
      if(hasLuaStartup)
      {
        result = call_function<bool>(luaStartup, this);
      }
      if(hasLuaProcessCallLater)
      {
        call_function<void>(luaProcessCallLater);
      }                
      return  result;
    }

    bool Tasklet::update()
    {
      bool result = true;
      if(hasLuaUpdate)
      {
        result = call_function<bool>(luaUpdate, this);
      }
      if(hasLuaProcessCallLater)
      {
        call_function<void>(luaProcessCallLater);
      }                      
      return  result;      
    }

    bool Tasklet::shutdown()
    {
      bool result = true;
      if(hasLuaShutdown)
      {
        result = call_function<bool>(luaShutdown, this);
      }
      if(hasLuaProcessCallLater)
      {
        call_function<void>(luaProcessCallLater);
      }                      
      return  result;      
    }

    bool Tasklet::start()
    {
      // init must've been called before this by application or some other external code
      return ::fhtagn::threads::tasklet::start();
    }

    bool Tasklet::stop()
    {
      bool result = ::fhtagn::threads::tasklet::stop();
      if (result)
      {
        eventDispatcher->wakeup();
      }
      return result;
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
    
  }
}