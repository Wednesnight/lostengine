#include "lost/application/Tasklet.h"
#include "lost/application/TaskletThread.h"
#include "lost/application/TaskletEvent.h"
#include "lost/application/Window.h"
#include "lost/event/EventDispatcher.h"
#include <sstream>
#include <stdexcept>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace std;

namespace lost
{
  namespace application
  {

    struct Tasklet::TaskletHiddenMembers
    {
      lost::shared_ptr<TaskletThread> thread;
      unsigned int threadId;
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
      hiddenMembers->threadId = GetCurrentThreadId();
      init();
      bool hasError = false;
      try
      {
        // make sure that our GL context is the current context
        if(window != NULL)
        {
          window->context->makeCurrent();
        }
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
      }
      catch (...)
      {
        hasError = true;
      }
      isAlive = false;
      dispatchApplicationEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), this)));
      cleanup();
      if (hasError)
      {
        ostringstream os;
        os << "Tasklet terminated with error: " << name;
        throw runtime_error(os.str());
      }
    }

    void Tasklet::stop()
    {
      if (isAlive)
      {
        hiddenMembers->thread->stop();
        // wakeup
        if (waitForEvents)
        {
          if (window) PostThreadMessage(hiddenMembers->threadId, WM_NOTIFY, 0, 0);
            else eventDispatcher->wakeup();
        }
        hiddenMembers->thread->wait();
      }
    }

  }
}
