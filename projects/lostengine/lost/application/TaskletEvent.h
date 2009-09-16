#ifndef LOST_APPLICATION_TASKLETEVENT_H
#define LOST_APPLICATION_TASKLETEVENT_H

#include "lost/event/Event.h"
#include "lost/application/Tasklet.h"

namespace lost
{
  namespace application
  {
    struct TaskletEvent;
    typedef lost::shared_ptr<TaskletEvent> TaskletEventPtr;

    struct TaskletEvent : public event::Event
    {
      static const event::Type& DONE() { static event::Type d = "taskletDone"; return d; }

      Tasklet* tasklet;

      TaskletEvent(const event::Type& inType, Tasklet* inTasklet) : Event(inType), tasklet(inTasklet) {}
      virtual ~TaskletEvent() {}
    };
  }
}

#endif
