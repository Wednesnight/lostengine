#ifndef LOST_APPLICATION_DEBUGEVENT_H
#define LOST_APPLICATION_DEBUGEVENT_H

#include "lost/event/Event.h"
#include "lost/application/forward.h"

namespace lost
{
  namespace application
  {
    struct DebugEvent : public event::Event
    {
      struct DebugInfo
      {
        int memSize;
      };

      static const event::Type& GET_MEM_INFO() { static event::Type d = "debugGetMemInfo"; return d; }
      static const event::Type& MEM_INFO() { static event::Type d = "debugMemInfo"; return d; }

      Tasklet* tasklet;
      DebugInfo info;

      DebugEvent(const event::Type& inType, Tasklet* inTasklet) : Event(inType), tasklet(inTasklet) {}
      virtual ~DebugEvent() {}

      static DebugEventPtr create(const event::Type& inType, Tasklet* inTasklet) { return DebugEventPtr(new DebugEvent(inType, inTasklet)); }
    };
  }
}

#endif
