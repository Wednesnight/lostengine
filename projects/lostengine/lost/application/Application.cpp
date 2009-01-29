#include "lost/application/Application.h"

namespace lost
{
  namespace application
  {
    
    Application::Application()
    {
      initialize();
    }

    Application::Application(const boost::function<void (void)>& inRunLoopFunction)
    {
      runLoop = boost::shared_ptr<RunLoop>(new RunLoopFunctor(inRunLoopFunction));
      initialize();
    }

    Application::Application(const boost::shared_ptr<RunLoop>& inRunLoop)
    {
      runLoop = inRunLoop;
      initialize();
    }

    Application::Application(const boost::filesystem::path& inRunLoopScript)
    {
      boost::shared_ptr<RunLoopScript> runLoopScript = boost::shared_ptr<RunLoopScript>(new RunLoopScript(inRunLoopScript));
      luabind::globals(*(runLoopScript->interpreter))["lost"]["globals"]["app"] = this; // map the app itself into the interpreter so scripts can attach to its events
      // try to load lua script
      try
      {
        boost::shared_ptr<lost::resource::File> initScript = runLoopScript->loader->load(inRunLoopScript);
        runLoopScript->interpreter->doFile(initScript);
      }
      catch(std::exception& ex)
      {
        EOUT("couldn't load script <"+ inRunLoopScript.native_file_string() +">: "+ std::string(ex.what()));
      }
      initialize();
    }

    Application::~Application()
    {
      finalize();
    }

  }
}
