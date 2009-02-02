#include "lost/application/Application.h"
#include "lost/resource/DefaultLoader.h"

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
    
    Application::~Application()
    {
      finalize();
    }

    void Application::setRunLoop(const boost::function<void (const boost::shared_ptr<Application>& sender)>& inRunLoop)
    {
      if (runLoopThread) runLoopThread->setRunLoop(inRunLoop);
    }
    
    boost::shared_ptr<Window> Application::createWindow(const std::string& uniqueId, const WindowParams& params)
    {
      boost::shared_ptr<Window> result = Window::create(shared_from_this(), params);
      windows[uniqueId] = result;
      return result;
    }
    
    void Application::run()
    {
      if (runLoopThread) runLoopThread->initialize(shared_from_this());
      boost::shared_ptr<ApplicationEvent> appEvent(new ApplicationEvent(ApplicationEvent::RUN()));
      dispatchEvent(appEvent);
      doRun();
    }
    
    void Application::startRunLoop()
    {
      if (runLoopThread) runLoopThread->run(shared_from_this());
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
      if (runLoopThread) runLoopThread->join();
    }

  }
}
