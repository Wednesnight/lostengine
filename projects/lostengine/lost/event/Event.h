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

#ifndef LOST_EVENT_EVENT_H
#define LOST_EVENT_EVENT_H

#include "lost/event/forward.h"
#include "lost/event/EventType.h"

namespace lost
{
  namespace event
  {
		/** low-level event class.
		 *	Any event that should be dispatched via the EventDispatcher must be derived from this class.
		 */
    struct Event 
    {    
      Type type;					// the type of the event
      int32_t refcount; // for Pool management, ignore
      // these are currently only used for guiro
      bool bubbles;
      bool stopPropagation;
      bool stopDispatch;

      const char* classTypeName() { static const char* d = typeid(this).name(); return d; }

      Event(const Type& inType) : type(inType),bubbles(false),stopPropagation(false),stopDispatch(false),refcount(0) {}
      static EventPtr create(const Type& inType) { return EventPtr(new Event(inType)); }
      virtual ~Event() {}
    };

  }
}

#endif
