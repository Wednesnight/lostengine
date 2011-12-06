#include "lost/application/Tasklet.h"
#include "lost/application/TaskletThread.h"
#include "lost/application/TaskletEvent.h"
#include "lost/application/Window.h"
#include "lost/event/EventDispatcher.h"
#include "lost/gl/Context.h"
#include "lost/platform/Platform.h"

#include <sstream>
#include <stdexcept>

#include <fhtagn/threads/tasklet.h>

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
      init();
      hiddenMembers.reset(new TaskletHiddenMembers);
      hiddenMembers->thread.reset(new TaskletThread(this));
      hiddenMembers->thread->start();
    }

    void Tasklet::run()
    {
      isAlive = true;
      hiddenMembers->threadId = GetCurrentThreadId();
      bool hasError = false;
      string errorMsg;
      try
      {
        // make sure that our GL context is the current context
        if(window != NULL)
        {
          window->context->makeCurrent();
        }

        startup();
		    if (running)
        {
          double framerate = config.framerate;
          double offset = clock.getTime();

          while (hiddenMembers->thread->get_state() == fhtagn::threads::tasklet::RUNNING && running)
          {
            processEvents();
            if (running) {
              update(framerate);
              render();

              if(waitForEvents)
              {
//                if (window) WaitMessage();
//                  else eventDispatcher->waitForEvents();
                eventDispatcher->waitForEvents();
              }
/*
              MSG msg;
              while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
              {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
              }
*/

              framerate = clock.getElapsedAndUpdateOffset(offset);
            }
          }
        }
      }
      catch (std::exception& e)
      {
        errorMsg = e.what();
        hasError = true;
      }

      isAlive = false;
      dispatchApplicationEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), this)));
      if (hasError)
      {
        ostringstream os;
        os << "Tasklet terminated with error: " << errorMsg;
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
//          if (window) PostThreadMessage(hiddenMembers->threadId, WM_NOTIFY, 0, 0);
//            else eventDispatcher->wakeup();
          eventDispatcher->wakeup();
        }
        hiddenMembers->thread->wait();
      }
      shutdown();
      cleanup();
    }

  }
}
