#ifndef LOST_EVENT_SIGNAL_H
#define LOST_EVENT_SIGNAL_H

#include "lost/event/forward.h"
#include "EASTL/map.h"


namespace lost
{
namespace event
{

struct Signal
{
  Signal();
  virtual ~Signal();
  
  uint32_t addListener(ListenerPtr listener);
  void removeListener(uint32_t index);
  
  void call(EventPtr event);
  
  uint32_t counter; // used for generating ids that can be used for connection tracking
  eastl::map<uint32_t, ListenerPtr> id2listener;
};

}
}

#endif
