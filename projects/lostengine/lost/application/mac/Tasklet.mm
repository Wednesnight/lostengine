#include "lost/application/Tasklet.h"
#include "lost/application/TaskletThread.h"
#include "lost/application/TaskletEvent.h"
#include "lost/event/EventDispatcher.h"

#include <Foundation/NSAutoreleasePool.h>

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
      init();
      hiddenMembers = new TaskletHiddenMembers;
      hiddenMembers->thread = new TaskletThread(this);
      isAlive = hiddenMembers->thread->start();
    }

    void Tasklet::run()
    {
      NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
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
      [pool drain];
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
