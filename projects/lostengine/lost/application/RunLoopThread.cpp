#include "lost/application/Application.h"
#include "lost/application/RunLoopThread.h"

using namespace lost::event;
using namespace lost::lua;
using namespace lost::platform;
using namespace lost::resource;
using namespace luabind;
using namespace std;

namespace lost
{
  namespace application
  {
    RunLoopThread::RunLoopThread()
    : running(false)
    {
    }

    RunLoopThread::RunLoopThread(const RunLoopFunction& inRunLoop)
    : running(false), waitForEvents(false)
    {
      setRunLoop(inRunLoop);
    }


    void RunLoopThread::loop()
    {
      running = true;
      while (running)
      {
        const double startTime = currentTimeMilliSeconds();
        if(!waitForEvents)
        {
          if (runLoop) runLoop(application);
          application->processEvents(fmax(0, 1.0/60.0 - currentTimeMilliSeconds() - startTime));
        }
        else
        {
          if (runLoop) runLoop(application);
          application->waitForEvents();
          application->processEvents(0); // no event processing timeout since we probably don't have that many events 
        }
      }
    }

    void RunLoopThread::quit(EventPtr event)
    {
      running = false;
    }

    void RunLoopThread::setRunLoop(const RunLoopFunction& inRunLoop)
    {
      runLoop = inRunLoop;
    }

    void RunLoopThread::initialize(const ApplicationPtr& inApplication)
    {
      application = inApplication;
      application->addEventListener(ApplicationEvent::QUIT(), boost::bind(&RunLoopThread::quit, this, _1));
    }
    
    void RunLoopThread::run(const ApplicationPtr& inApplication)
    {
      thread.reset(new boost::thread(boost::bind(&RunLoopThread::loop, this)));
    }

    void RunLoopThread::join()
    {
      if(thread && thread->get_id() != boost::this_thread::get_id())
      {
        thread->join();
      }
    }

    RunLoopThreadLua::RunLoopThreadLua(const boost::filesystem::path& inFilename)
    : filename(inFilename)
    {
    }    

    void RunLoopThreadLua::initialize(const ApplicationPtr& inApplication)
    {
      RunLoopThread::initialize(inApplication);

      interpreter.reset(new State(inApplication->loader));  // init lua state with resource loader
      bindAll(*interpreter);                                // bind lostengine lua mappings    
      
      /**
       * each runloop interpreter has got its own environment reference including the following objects:
       *   - interpreter
       *   - application
       */
      globals(*interpreter)["environment"] = newtable(*interpreter);
      // map the state itself into the interpreter so that
      //   - error handling is working and
      //   - scripts can use it
      globals(*interpreter)["environment"]["interpreter"] = interpreter;
      // map the loader into the interpreter so that scripts can load resources
      globals(*interpreter)["environment"]["application"] = inApplication;
      
      ModuleLoader::install(*interpreter, inApplication->loader); // install custom module loader so require goes through resourceLoader
      
      // try to load lua script
      try
      {
        FilePtr initScript = inApplication->loader->load(filename);
        interpreter->doFile(initScript);
        object main = globals(*interpreter)["main"];
        if (type(main) != LUA_TFUNCTION) throw runtime_error("main() is not defined");
        call_function<void>(main, inApplication);
      }
      catch(exception& ex)
      {
        EOUT("couldn't load script <"+ filename.native_file_string() +">: "+ string(ex.what()));
      }
    }

  }
}
