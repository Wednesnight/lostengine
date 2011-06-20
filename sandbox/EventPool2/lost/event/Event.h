#ifndef LOST_EVENT_EVENT_H
#define LOST_EVENT_EVENT_H

#include "lost/event/forward.h"
#include "tinythread.h"

namespace lost
{
  namespace event
  {
    typedef string Type;

		/** low-level event class.
		 *	Any event that should be dispatched via the EventDispatcher must be derived from this class.
		 */
    struct Event 
    {    
      Type      type;					// the type of the event
      uint32_t  refcount;
      tthread::mutex _mutex;
      void incRef()
      { 
        tthread::lock_guard<tthread::mutex> lock(_mutex);
        refcount++;
      }

      void decRef()
      { 
        tthread::lock_guard<tthread::mutex> lock(_mutex);
        refcount--;
      }

      Event(const Type& inType) : type(inType),refcount(0) {}
      static EventPtr create(const Type& inType) { return EventPtr(new Event(inType)); }
      virtual ~Event() {}
    };

  }
}

#endif
