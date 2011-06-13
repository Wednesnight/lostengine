#include "lost/event/Signal.h"
#include "lost/event/Listener.h"

namespace lost
{
namespace event
{

Signal::Signal()
{
  counter = 0;
}

Signal::~Signal()
{
}
  
uint32_t Signal::addListener(ListenerPtr listener)
{
  uint32_t result = counter;
  ++counter;
  
  id2listener[result] = listener;
  
  return result;
}

void Signal::call(EventPtr event)
{
  eastl::map<uint32_t, ListenerPtr>::iterator i;
  for(i = id2listener.begin(); i!= id2listener.end(); ++i)
  {
    i->second->call(event);
  }
}

}
}