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
    : running(false)
    {
      setRunLoop(inRunLoop);
    }

    void RunLoopThread::loop()
    {
      running = true;
      while (running)
      {
        if (runLoop) runLoop(application);
        application->processEvents();
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

    void RunLoopThread::run(const boost::shared_ptr<Application>& inApplication)
    {
      application = inApplication;
      application->addEventListener(ApplicationEvent::QUIT(), boost::bind(&RunLoopThread::quit, this, _1));
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

    void RunLoopThreadLua::run(const boost::shared_ptr<Application>& inApplication)
    {
      interpreter.reset(new lua::State(true, true, true, inApplication->loader));  // init lua state with resource loader
      lost::lua::bindAll(*interpreter);                                            // bind lostengine lua mappings    

      // map the state itself into the interpreter so that
      //   - error handling is working and
      //   - scripts can use it
      luabind::globals(*interpreter)["lost"]["lua"]["currentState"] = interpreter;
      // map the loader into the interpreter so that scripts can load resources
      luabind::globals(*interpreter)["lost"]["resource"]["currentLoader"] = inApplication->loader;

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
      RunLoopThread::run(inApplication);
    }

  }
}
