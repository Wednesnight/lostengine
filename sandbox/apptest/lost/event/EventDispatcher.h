#ifndef LOST_EVENT_EVENTDISPATCHER_H
#define LOST_EVENT_EVENTDISPATCHER_H

#include "lost/event/forward.h"
#include "EASTL/map.h"

namespace lost
{
namespace event
{

struct EventDispatcher
{
  EventDispatcher();
  virtual ~EventDispatcher();
  
  eastl::map<event::Type, Signal> eventType2signal;
};

}
}

#endif