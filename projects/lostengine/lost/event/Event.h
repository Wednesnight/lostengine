#ifndef LOST_EVENT_EVENT_H
#define LOST_EVENT_EVENT_H

#include <string>

namespace lost
{
  namespace event
  {
    struct Event;
    typedef lost::shared_ptr<Event> EventPtr;

    typedef std::string Type;

		/** low-level event class.
		 *	Any event that should be dispatched via the EventDispatcher must be derived from this class.
		 */
    struct Event : public lost::enable_shared_from_this<Event>
    {    
      Type type;					// the type of the event

      Event(const Type& inType) : type(inType) {}
      virtual ~Event() {}
    };

  }
}

#endif
