#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/Tasklet.h"
#include "lost/application/SpawnTaskletEvent.h"
#include "lost/common/Logger.h"
#include "lost/event/EventDispatcher.h"

#include <iostream>
#include <stdexcept>

using namespace std;

namespace lost
{
  namespace application
  {

    struct Application::ApplicationHiddenMembers
    {
      boost::condition condition;
    };

    void Application::initialize()
    {
      DOUT("Application::initialize()");

      // initialize hiddenMembers
      hiddenMembers = new ApplicationHiddenMembers;
    }

    void Application::finalize()
    {
      DOUT("Application::finalize()");
      delete hiddenMembers;
    }

    void Application::run(int argc, char *argv[])
    {
      DOUT("Application::run()");

      ApplicationEventPtr event = ApplicationEvent::create(ApplicationEvent::RUN());
      eventDispatcher->dispatchEvent(event);

      boost::mutex applicationMutex;
      boost::unique_lock<boost::mutex> applicationLock(applicationMutex);
      while (running)
      {
        hiddenMembers->condition.wait(applicationLock);
        eventDispatcher->processEvents();
      }
    }

    void Application::shutdown()
    {
      hiddenMembers->condition.notify_one();
    }

    void Application::showMouse(bool visible)
    {
    }

    void Application::processEvents(const ProcessEventPtr& event)
    {
      hiddenMembers->condition.notify_one();
    }

    void Application::taskletSpawn(const SpawnTaskletEventPtr& event)
    {
      addTasklet(new Tasklet(event->loader, event->writer));
    }

  }
}

