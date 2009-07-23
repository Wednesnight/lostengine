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
      eventDispatcher->clear();
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
      luabind::object obj = lua->globals["hasWindow"];
      if(obj) {hasWindow = object_cast<bool>(obj);}
      obj = lua->globals["windowParams"];
      if(obj) {windowParams=object_cast<WindowParams>(obj);}
      
      // create window if flag is set
      if(hasWindow)
      {
        window = Window::create(eventDispatcher, windowParams);
        window->open();        
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

    void Tasklet::run(tasklet& tasklet)
    {
      void* pool = threadAutoreleasePoolHack_createPool();

      // make sure that our GL context is the current context
      if(hasWindow)
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
      eventDispatcher->dispatchEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), this)));
    }

    void Tasklet::error(fhtagn::threads::tasklet& tasklet, std::exception const& exception)
    {
      EOUT(exception.what());
      eventDispatcher->dispatchEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), this)));
    }

    bool Tasklet::startup()
    {    
      bool result = true;
      if(hasLuaStartup)
      {
        result = call_function<bool>(luaStartup, shared_from_this());
      }
      return  result;
    }

    bool Tasklet::update()
    {
      bool result = true;
      if(hasLuaUpdate)
      {
        result = call_function<bool>(luaUpdate, shared_from_this());
      }
      return  result;      
    }

    bool Tasklet::shutdown()
    {
      bool result = true;
      if(hasLuaShutdown)
      {
        result = call_function<bool>(luaShutdown, shared_from_this());
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
      return ::fhtagn::threads::tasklet::stop();
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
