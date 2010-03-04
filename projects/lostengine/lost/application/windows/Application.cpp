#include <iostream>

#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/MultiThreadedTasklet.h"
#include "lost/application/SpawnTaskletEvent.h"
#include "lost/common/Logger.h"
#include "lost/event/EventDispatcher.h"
#include <windows.h>

namespace lost
{
namespace application
{
struct Application::ApplicationHiddenMembers
{
  bool running;
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

void Application::run()
{
  DOUT("Application::run()");

  ApplicationEventPtr event = ApplicationEvent::create(ApplicationEvent::RUN());
  eventDispatcher->dispatchEvent(event);

  MSG msg;
  hiddenMembers->running = true;
  while (hiddenMembers->running)
  {
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      switch(msg.message)
      {
        case WM_QUIT:
          hiddenMembers->running = false;
          break;

        default:
          DispatchMessage( &msg );
          break;
      }
    }
  }
  quit();
}

void Application::shutdown()
{
}

void Application::showMouse(bool visible)
{
}

void Application::processEvents(const ProcessEventPtr& event)
{
}

void Application::taskletSpawn(const SpawnTaskletEventPtr& event)
{
  addTasklet(new MultiThreadedTasklet(event->loader));
}

}
}
