#include "lost/application/Application.h"
#include "lost/resource/DefaultLoader.h"

#include <boost/program_options.hpp>

namespace lost
{
  namespace application
  {
    
    Application::Application()
    : loader(new lost::resource::DefaultLoader),
      runLoopThread(new RunLoopThread)
    {
      initialize();
    }

    Application::Application(const boost::function<void (const boost::shared_ptr<Application>& sender)>& inRunLoop)
    : loader(new lost::resource::DefaultLoader),
      runLoopThread(new RunLoopThread(inRunLoop))
    {
      initialize();
    }

    Application::Application(const boost::filesystem::path& inScript)
    : loader(new lost::resource::DefaultLoader),
      runLoopThread(new RunLoopThreadLua(inScript))
    {
      initialize();
    }

    Application::Application(int argn, char** args)
    : loader(new lost::resource::DefaultLoader)
    {
      using namespace boost::program_options;

      // Declare the supported options.
      options_description desc;
      desc.add_options()
        ("script", value<std::string>())
      ;
      positional_options_description p;
      p.add("script", -1);
      
      variables_map vm;
      store(command_line_parser(argn, args).options(desc).positional(p).run(), vm);
      notify(vm);
      
      if (vm.count("script"))
      {
        runLoopThread.reset(new RunLoopThreadLua(vm["script"].as<std::string>()));
      }
      initialize();
    }
    
    boost::shared_ptr<Application> Application::create()
    {
      return boost::shared_ptr<Application>(new Application);
    }

    boost::shared_ptr<Application> Application::create(const boost::function<void (const boost::shared_ptr<Application>& sender)>& inRunLoop)
    {
      return boost::shared_ptr<Application>(new Application(inRunLoop));
    }

    boost::shared_ptr<Application> Application::create(const boost::filesystem::path& inScript)
    {
      return boost::shared_ptr<Application>(new Application(inScript));
    }

    boost::shared_ptr<Application> Application::create(int argn, char** args)
    {
      return boost::shared_ptr<Application>(new Application(argn, args));
    }
    
    void Application::runLoopWaitsForEvents(bool flag)
    {
      runLoopThread->waitForEvents = flag;
    }
    
    Application::~Application()
    {
      finalize();
    }

    void Application::setRunLoop(const boost::function<void (const boost::shared_ptr<Application>& sender)>& inRunLoop)
    {
      runLoopThread->setRunLoop(inRunLoop);
    }
    
    boost::shared_ptr<Window> Application::createWindow(const std::string& uniqueId, const WindowParams& params)
    {
      boost::shared_ptr<Window> result = Window::create(shared_from_this(), params);
      windows[uniqueId] = result;
      return result;
    }
    
    void Application::run()
    {
      if (!runLoopThread) throw std::runtime_error("you have to set a runloop");
      runLoopThread->initialize(shared_from_this());
      boost::shared_ptr<ApplicationEvent> appEvent(new ApplicationEvent(ApplicationEvent::RUN()));
      dispatchEvent(appEvent);
      doRun();
    }
    
    void Application::startRunLoop()
    {
      runLoopThread->run(shared_from_this());
    }
    
    void Application::quit()
    {
      boost::shared_ptr<ApplicationEvent> appEvent(new ApplicationEvent(ApplicationEvent::QUIT()));
      queueEvent(appEvent);
      doQuit();
    }
    
    void Application::terminate()
    {
      boost::shared_ptr<ApplicationEvent> appEvent(new ApplicationEvent(ApplicationEvent::TERMINATE()));
      queueEvent(appEvent);
      runLoopThread->join();
    }

  }
}
