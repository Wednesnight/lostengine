#include "lost/application/Application.h"
#include "lost/application/RunLoopThread.h"

namespace lost
{
  namespace application
  {
    RunLoopThread::RunLoopThread()
    : running(false)
    {
    }

    RunLoopThread::RunLoopThread(const boost::function<void (const boost::shared_ptr<Application>& sender)>& inRunLoop)
    : running(false), waitForEvents(false)
    {
      setRunLoop(inRunLoop);
    }


    void RunLoopThread::loop()
    {
      running = true;
      while (running)
      {
        const double startTime = lost::platform::currentTimeMilliSeconds();
        if(!waitForEvents)
        {
          if (runLoop) runLoop(application);
          application->processEvents(fmax(0, 1.0/60.0 - lost::platform::currentTimeMilliSeconds() - startTime));
        }
        else
        {
          if (runLoop) runLoop(application);
          application->waitForEvents();
          application->processEvents(0); // no event processing timeout since we probably don't have that many events 
        }
      }
    }

    void RunLoopThread::quit(boost::shared_ptr<event::Event> event)
    {
      running = false;
    }

    void RunLoopThread::setRunLoop(const boost::function<void (const boost::shared_ptr<Application>& sender)>& inRunLoop)
    {
      runLoop = inRunLoop;
    }

    void RunLoopThread::initialize(const boost::shared_ptr<Application>& inApplication)
    {
      application = inApplication;
      application->addEventListener(ApplicationEvent::QUIT(), boost::bind(&RunLoopThread::quit, this, _1));
    }
    
    void RunLoopThread::run(const boost::shared_ptr<Application>& inApplication)
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

    void RunLoopThreadLua::initialize(const boost::shared_ptr<Application>& inApplication)
    {
      RunLoopThread::initialize(inApplication);

      interpreter.reset(new lua::State(inApplication->loader));  // init lua state with resource loader
      lost::lua::bindAll(*interpreter);                                            // bind lostengine lua mappings    
      
      /**
       * each runloop interpreter has got its own environment reference including the following objects:
       *   - interpreter
       *   - application
       */
      luabind::globals(*interpreter)["environment"] = luabind::newtable(*interpreter);
      // map the state itself into the interpreter so that
      //   - error handling is working and
      //   - scripts can use it
      luabind::globals(*interpreter)["environment"]["interpreter"] = interpreter;
      // map the loader into the interpreter so that scripts can load resources
      luabind::globals(*interpreter)["environment"]["application"] = inApplication;
      
      lost::lua::ModuleLoader::install(*interpreter, inApplication->loader); // install custom module loader so require goes through resourceLoader
      
      // try to load lua script
      try
      {
        boost::shared_ptr<lost::resource::File> initScript = inApplication->loader->load(filename);
        interpreter->doFile(initScript);
        luabind::object main = luabind::globals(*interpreter)["main"];
        if (luabind::type(main) != LUA_TFUNCTION) throw std::runtime_error("main() is not defined");
        luabind::call_function<void>(main, inApplication);
      }
      catch(std::exception& ex)
      {
        EOUT("couldn't load script <"+ filename.native_file_string() +">: "+ std::string(ex.what()));
      }
    }

  }
}
