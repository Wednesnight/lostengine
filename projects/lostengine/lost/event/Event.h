#ifndef LOST_EVENT_EVENT_H
#define LOST_EVENT_EVENT_H

#include <string>

namespace lost
{
  namespace event
  {
    typedef std::string Type;

    struct Event
    {
    
      Type type;

      Event(const Type& inType) : type(inType) {}
      virtual ~Event() {}
    };

  }
}

#endif
