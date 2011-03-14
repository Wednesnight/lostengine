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
        lua_Debug* debug;
        int memSize;
      };

      static const event::Type& CMD_MEM_INFO() { static event::Type d = "debugCmdMemInfo"; return d; }
      static const event::Type& CMD_PAUSE() { static event::Type d = "debugCmdPause"; return d; }
      static const event::Type& CMD_CONTINUE() { static event::Type d = "debugCmdContinue"; return d; }

      static const event::Type& MEM_INFO() { static event::Type d = "debugMemInfo"; return d; }
      static const event::Type& PAUSE() { static event::Type d = "debugPause"; return d; }

      int mode;
      Tasklet* tasklet;
      DebugInfo info;

      DebugEvent(const event::Type& inType, Tasklet* inTasklet) : Event(inType), mode(0), tasklet(inTasklet) {}
      virtual ~DebugEvent() {}

      static DebugEventPtr create(const event::Type& inType, Tasklet* inTasklet) { return DebugEventPtr(new DebugEvent(inType, inTasklet)); }
    };
  }
}

#endif
