#ifndef LOST_APPLICATION_TASKLETEVENT_H
#define LOST_APPLICATION_TASKLETEVENT_H

#include "lost/event/Event.h"
#include "lost/application/forward.h"

namespace lost
{
  namespace application
  {
    struct TaskletEvent : public event::Event
    {
      static const event::Type& START() { static event::Type d = "taskletStart"; return d; }
      static const event::Type& TERMINATE() { static event::Type d = "taskletTerminate"; return d; }
      static const event::Type& DONE() { static event::Type d = "taskletDone"; return d; }

      Tasklet* tasklet;

      TaskletEvent(const event::Type& inType, Tasklet* inTasklet) : Event(inType), tasklet(inTasklet) {}
      virtual ~TaskletEvent() {}

      static TaskletEventPtr create(const event::Type& inType, Tasklet* inTasklet) { return TaskletEventPtr(new TaskletEvent(inType, inTasklet)); }
    };
  }
}

#endif
