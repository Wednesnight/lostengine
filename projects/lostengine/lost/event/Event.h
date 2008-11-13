#ifndef LOST_EVENT_EVENT_H
#define LOST_EVENT_EVENT_H

#include <string>
#include <boost/enable_shared_from_this.hpp>

namespace lost
{
  namespace event
  {
    typedef std::string Type;

    struct Event : public boost::enable_shared_from_this<Event>
    {
    
      Type type;

      Event(const Type& inType) : type(inType) {}
      virtual ~Event() {}
    };

  }
}

#endif
