#include "Application.h"

Application::Application()
{
  initialize();
}

Application::Application(boost::function<void (void)> mainLoopFunc)
{
  mainLoop = boost::shared_ptr<MainLoop>(new FunctorMainLoop(mainLoopFunc));
  initialize();
}

Application::Application(boost::shared_ptr<MainLoop> inMainLoop)
{
  mainLoop = inMainLoop;
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
