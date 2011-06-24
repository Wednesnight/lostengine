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
