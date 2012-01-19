/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef LOST_APPLICATION_DEBUGEVENT_H
#define LOST_APPLICATION_DEBUGEVENT_H

#include "lost/event/Event.h"
#include "lost/application/forward.h"
#include "lost/common/Hash.h"

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

      static const event::Type& CMD_MEM_INFO() { static event::Type d = common::djb2Hash("debugCmdMemInfo"); return d; }
      static const event::Type& CMD_PAUSE() { static event::Type d = common::djb2Hash("debugCmdPause"); return d; }
      static const event::Type& CMD_CONTINUE() { static event::Type d = common::djb2Hash("debugCmdContinue"); return d; }

      static const event::Type& MEM_INFO() { static event::Type d = common::djb2Hash("debugMemInfo"); return d; }
      static const event::Type& PAUSE() { static event::Type d = common::djb2Hash("debugPause"); return d; }

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
