#include "lost/application/Tasklet.h"
#include "lost/application/TaskletThread.h"
#include "lost/application/TaskletEvent.h"
#include "lost/application/Window.h"
#include "lost/event/EventDispatcher.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace lost
{
  namespace application
  {

    struct Tasklet::TaskletHiddenMembers
    {
      lost::shared_ptr<TaskletThread> thread;
    };

    void Tasklet::start()
    {
      hiddenMembers.reset(new TaskletHiddenMembers);
      hiddenMembers->thread.reset(new TaskletThread(this));
      hiddenMembers->thread->start();
    }

    void Tasklet::run()
    {
      isAlive = true;
      init();
      if (startup())
      {
        while (hiddenMembers->thread->get_state() == fhtagn::threads::tasklet::RUNNING && update())
        {
          render();

          if(waitForEvents)
          {
            if (window) WaitMessage();
              else eventDispatcher->waitForEvents();
          }
          MSG msg;
          while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
          {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
          }
          processEvents();
        }
        shutdown();
      }
      dispatchApplicationEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), this)));
      cleanup();
      isAlive = false;
    }

    void Tasklet::stop()
    {
      if (isAlive)
      {
        hiddenMembers->thread->stop();
        // wakeup
        if (waitForEvents)
        {
          if (window) window->close();
            else eventDispatcher->wakeup();
        }
        hiddenMembers->thread->wait();
      }
    }

  }
}
