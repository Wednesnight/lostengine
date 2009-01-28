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
      runLoop = boost::shared_ptr<RunLoop>(new RunLoopScript(inRunLoopScript));
      initialize();
    }

    Application::~Application()
    {
      finalize();
    }

    void Application::queueEvent(const boost::shared_ptr<lost::event::Event>& event)
    {
      queueMutex.lock();
      if (!eventQueue) eventQueue.reset(new std::list<boost::shared_ptr<lost::event::Event> >());
      eventQueue->push_back(event);
      queueMutex.unlock();
    }

    void Application::processEvents(const double& timeoutInSeconds)
    {
      if (eventQueue)
      {
        boost::shared_ptr<std::list<boost::shared_ptr<lost::event::Event> > > currentQueue = eventQueue;
        queueMutex.lock();
        eventQueue.reset();
        queueMutex.unlock();
        for (std::list<boost::shared_ptr<lost::event::Event> >::iterator idx = currentQueue->begin(); idx != currentQueue->end(); ++idx)
          dispatchEvent(*idx);
      }
    }

  }
}
