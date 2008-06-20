#ifndef LOST_GUIRO_EVENT_EVENT_H
#define LOST_GUIRO_EVENT_EVENT_H

#include "lost/event/EventTypes.h"

namespace lost
{
namespace event
{

  struct Event
  {
    EventType type;

    Event() {}
    virtual ~Event() {}
  };

}
}

#endif
