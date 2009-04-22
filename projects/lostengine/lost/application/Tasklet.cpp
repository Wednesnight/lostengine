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
      executeScript(true),
      loader(inLoader),
      eventDispatcher(new EventDispatcher()),
      interpreter(new State(loader)),
      config(new Config(interpreter)),
      waitForEvents(false),
      script("main.lua")
    {
      // set error handler
      add_error_handler(bind(&Tasklet::error, this, _1, _2));

      // bind lostengine lua mappings
      bindAll(*interpreter);
      // install custom module loader so require goes through resourceLoader
      ModuleLoader::install(*interpreter, loader);
    }
    
    Tasklet::~Tasklet()
    {
      eventDispatcher->clear();
    }
    
    void Tasklet::run(tasklet& tasklet)
    {
      threadAutoreleasePoolHack_createPool();
      try
      {
        executeScript = true;
        interpreter->doFile(script);
      }
      catch(std::exception& ex)
      {
        DOUT("couldn't load script <"+ script +">: "+ string(ex.what()));
        executeScript = false;
      }

      if (startup())
      {
        while (get_state() == RUNNING && main())
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
          threadAutoreleasePoolHack_drainAndRecreatePool();
        }
        shutdown();
      }
      threadAutoreleasePoolHack_drainPool();        
      application->notifyTaskletDeath(this);
    }

    bool Tasklet::callScriptFunction(const string& funcname)
    {
      bool result = true;
      if (executeScript) 
      {
        object func = globals(*interpreter)[funcname];
        if (luabind::type(func) != LUA_TFUNCTION)
        {
          WOUT("no " << funcname << "() found in <" << script << ">");
        }
        else
        {
          result = call_function<bool>(func, shared_from_this());
        }
      }
      return result;
    }

    void Tasklet::error(fhtagn::threads::tasklet& tasklet, std::exception const& exception)
    {
      EOUT(exception.what());
      application->notifyTaskletDeath(this, exception);
    }

    bool Tasklet::startup()
    {
      return callScriptFunction("startup");
    }

    bool Tasklet::main()
    {
      return callScriptFunction("main");
    }

    bool Tasklet::shutdown()
    {
      return callScriptFunction("shutdown");
    }

    bool Tasklet::start()
    {
      return ::fhtagn::threads::tasklet::start();
    }

    bool Tasklet::stop()
    {
      return ::fhtagn::threads::tasklet::stop();
    }

  }
}
