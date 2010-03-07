#include "lost/application/Tasklet.h"
#include "lost/application/TaskletThread.h"
#include "lost/application/TaskletEvent.h"
#include "lost/event/EventDispatcher.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace lost
{
  namespace application
  {

    struct Tasklet::TaskletHiddenMembers
    {
      TaskletThread* thread;
    };

    void Tasklet::start()
    {
      hiddenMembers = new TaskletHiddenMembers;
      hiddenMembers->thread = new TaskletThread(this);
      isAlive = hiddenMembers->thread->start();
    }

    void Tasklet::run()
    {
      init();
      if (startup())
      {
        while (hiddenMembers->thread->get_state() == fhtagn::threads::tasklet::RUNNING && update())
        {
          render();

          if(waitForEvents)
          {
            WaitMessage();
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
    }

    void Tasklet::stop()
    {
      isAlive = !hiddenMembers->thread->stop();
      if (!isAlive)
      {
        hiddenMembers->thread->wait();
        delete hiddenMembers;
      }
    }

  }
}
