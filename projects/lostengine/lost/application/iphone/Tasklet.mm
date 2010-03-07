#include "lost/application/Tasklet.h"
#include "lost/application/TaskletEvent.h"
#include "lost/event/EventDispatcher.h"

#include <Foundation/NSAutoreleasePool.h>

namespace lost
{
  namespace application
  {

    struct Tasklet::TaskletHiddenMembers
    {
      // CADisplayLink ref (wrapper?)
    };

    void Tasklet::start()
    {
      init();
      // initialize CADisplayLink
      isAlive = true;
    }

    void Tasklet::run()
    {
      NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
      if (startup())
      {
        while (/* check own status */true && update())
        {
          render();

          /* we probably don't need this on iphone
          if(waitForEvents)
          {
            eventDispatcher->waitForEvents();
          }
          */
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
      // cleanup CADisplayLink
      isAlive = false;
    }

  }
}
