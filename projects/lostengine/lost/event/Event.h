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
      
      // these are currently only used for guiro
      bool bubbles;
      bool stopPropagation;
      bool stopDispatch;

      Event(const Type& inType) : type(inType),bubbles(false),stopPropagation(false),stopDispatch(false) {}
      static EventPtr create(const Type& inType) { return EventPtr(new Event(inType)); }
      virtual ~Event() {}
    };

  }
}

#endif
