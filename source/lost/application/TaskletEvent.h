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

#ifndef LOST_APPLICATION_TASKLETEVENT_H
#define LOST_APPLICATION_TASKLETEVENT_H

#include "lost/event/Event.h"
#include "lost/application/forward.h"
#include "lost/common/Hash.h"

namespace lost
{
  namespace application
  {
    struct TaskletEvent : public event::Event
    {
      static const event::Type& START() { static event::Type d = common::djb2Hash("taskletStart"); return d; }
      static const event::Type& TERMINATE() { static event::Type d = common::djb2Hash("taskletTerminate"); return d; }
      static const event::Type& DONE() { static event::Type d = common::djb2Hash("taskletDone"); return d; }

      Tasklet* tasklet;

      TaskletEvent(const event::Type& inType, Tasklet* inTasklet) : Event(inType), tasklet(inTasklet) {}
      virtual ~TaskletEvent() {}

      static TaskletEventPtr create(const event::Type& inType, Tasklet* inTasklet) { return TaskletEventPtr(new TaskletEvent(inType, inTasklet)); }
    };
  }
}

#endif
