#include "lost/application/Tasklet.h"
#include "lost/application/TaskletThread.h"
#include "lost/application/TaskletEvent.h"
#include "lost/event/EventDispatcher.h"
#include <sstream>
#include <stdexcept>

#include <Foundation/NSAutoreleasePool.h>

using namespace std;

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
      init();
      hiddenMembers.reset(new TaskletHiddenMembers);
      hiddenMembers->thread.reset(new TaskletThread(this));
      hiddenMembers->thread->start();
    }

    void Tasklet::run()
    {
      isAlive = true;
      NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
      bool hasError = false;
      try
      {
        if (startup())
        {
          while (hiddenMembers->thread->get_state() == fhtagn::threads::tasklet::RUNNING && update())
          {
            render();

            if(waitForEvents)
            {
              eventDispatcher->waitForEvents();
            }
            processEvents();

            [pool drain];
            pool = [[NSAutoreleasePool alloc] init];  
          }
          shutdown();
        }
      }
      catch (...)
      {
        hasError = true;
      }
      dispatchApplicationEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), this)));
      [pool drain];
      isAlive = false;
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
        if (waitForEvents) eventDispatcher->wakeup();
        hiddenMembers->thread->wait();
      }
      cleanup();
    }

  }
}
