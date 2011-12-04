#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/Tasklet.h"
#include "lost/application/SpawnTaskletEvent.h"
#include "lost/common/Logger.h"
#include "lost/event/EventDispatcher.h"
#include "lost/platform/Platform.h"

#include <iostream>
#include <stdexcept>
#include <boost/thread/condition.hpp>

using namespace std;

namespace lost
{
  namespace application
  {

    struct Application::ApplicationHiddenMembers
    {
//      boost::condition condition;
    };

    Application* currentApplication = NULL;
    BOOL WINAPI ConsoleHandler(DWORD event)
    {
      if (currentApplication != NULL) currentApplication->quit();
      return TRUE;
    }

    void Application::initialize()
    {
      DOUT("Application::initialize()");

      // setup console handler
      currentApplication = this;
      if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE) == FALSE)
      {
        // unable to install handler for console breaks
        throw runtime_error("SetConsoleCtrlHandler() failed");
      }

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

//      boost::mutex applicationMutex;
//      boost::unique_lock<boost::mutex> applicationLock(applicationMutex);
      while (running && WaitMessage()) {
//        hiddenMembers->condition.wait(applicationLock);
        MSG msg;
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
        eventDispatcher->processEvents();
      }
    }

    void Application::shutdown()
    {
//      hiddenMembers->condition.notify_one();
      PostThreadMessage(GetCurrentThreadId(), WM_NULL, 0, 0);
    }

    void Application::showMouse(bool visible)
    {
    }

    void Application::processEvents(const ProcessEventPtr& event)
    {
//      hiddenMembers->condition.notify_one();
      PostThreadMessage(GetCurrentThreadId(), WM_NULL, 0, 0);
    }

    void Application::taskletSpawn(const SpawnTaskletEventPtr& event)
    {
      addTasklet(new Tasklet(event->loader));
    }

  }
}
