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
#include "lost/application/mac/ThreadAutoreleasePoolHack.h"
#include "lost/application/TaskletEvent.h"
#include "lost/application/Queue.h"
#include "lost/application/QueueEvent.h"
#include "lost/application/ProcessEvent.h"
#include "lost/application/WindowEvent.h"
#include "lost/event/Receive.h"
#include "lost/rg/Node.h"
#include "lost/application/Window.h"

using namespace boost;
using namespace fhtagn::threads;
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
    : tasklet::tasklet(bind(&Tasklet::run, this, _1)),
      loader(inLoader),
      eventDispatcher(new EventDispatcher()),
      lua(new State(loader)),
      waitForEvents(false),
      name("<unnamed tasklet>"),
      renderNode(new Node()),
      updateQueue(new Queue())
    {
      // set error handler
      add_error_handler(bind(&Tasklet::error, this, _1, _2));

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
        windowParams=object_cast<WindowParams>(obj);
        window = new Window(eventDispatcher, windowParams);
        window->dispatcher->addEventListener(WindowEvent::CLOSE(), event::receive<WindowEvent>(bind(&Tasklet::closeWindow, this, _1)));
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
      if(window)
      {
        window->context->makeCurrent();
      }
        
      if (startup())
      {
        // process run loop updates
        updateQueue->process(this);
        while (get_state() == RUNNING && update())
        {
          // process run loop updates
          updateQueue->process(this);

          // render
          if(window)
          {
            renderNode->process(window->context);
            window->context->swapBuffers();
          }

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
      return  result;
    }

    bool Tasklet::update()
    {
      bool result = true;
      if(hasLuaUpdate)
      {
        result = call_function<bool>(luaUpdate, this);
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
