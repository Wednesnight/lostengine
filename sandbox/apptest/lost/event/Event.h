#ifndef LOST_EVENT_EVENT_H
#define LOST_EVENT_EVENT_H

#include "EASTL/string.h"
#include "lost/event/forward.h"

namespace lost
{
  namespace event
  {
    typedef eastl::string Type;

		/** low-level event class.
		 *	Any event that should be dispatched via the EventDispatcher must be derived from this class.
		 */
    struct Event 
    {    
      Type type;					// the type of the event

      Event(const Type& inType) : type(inType) {}
      static EventPtr create(const Type& inType) { return EventPtr(new Event(inType)); }
      virtual ~Event() {}
    };

  }
}

#endif
