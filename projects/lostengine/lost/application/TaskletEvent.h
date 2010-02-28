#ifndef LOST_APPLICATION_TASKLETEVENT_H
#define LOST_APPLICATION_TASKLETEVENT_H

#include "lost/event/Event.h"

namespace lost
{
  namespace application
  {
    struct TaskletEvent;
    typedef lost::shared_ptr<TaskletEvent> TaskletEventPtr;

    struct TaskletEvent : public event::Event
    {
      static const event::Type& DONE() { static event::Type d = "taskletDone"; return d; }

      void* tasklet; // sender must guarantee this is a Tasklet* because it will be cast in application
                     // we must use void* because of dependencies and complicated inheritance structure
                     // due to complicated cross-platform Tasklet.

      TaskletEvent(const event::Type& inType, void* inTasklet) : Event(inType), tasklet(inTasklet) {}
      virtual ~TaskletEvent() {}
    };
  }
}

#endif
