#ifndef LOST_GUIRO_EVENT_EVENT_H
#define LOST_GUIRO_EVENT_EVENT_H

#include <string>

namespace lost
{
namespace event
{
  typedef std::string EventType;

  struct Event
  {
  
    EventType type;

    Event(const EventType& inType) : type(inType) {}
    virtual ~Event() {}
  };

}
}

#endif
