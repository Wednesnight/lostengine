#ifndef LOST_EVENT_EVENT_H
#define LOST_EVENT_EVENT_H

#include <string>
#include <boost/enable_shared_from_this.hpp>

namespace lost
{
  namespace event
  {
    typedef std::string Type;

		/** low-level event class.
		 *	Any event that should be dispatched via the EventDispatcher must be derived from this class.
		 */
    struct Event : public boost::enable_shared_from_this<Event>
    {    
      Type type;					// the type of the event

      Event(const Type& inType) : type(inType) {}
      virtual ~Event() {}
    };

  }
}

#endif
