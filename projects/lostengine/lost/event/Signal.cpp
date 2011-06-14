#include "lost/event/Signal.h"
#include "lost/event/Listener.h"

using namespace std;

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

void Signal::removeListener(uint32_t index)
{
  map<uint32_t, ListenerPtr>::iterator pos = id2listener.find(index);
  if(pos != id2listener.end())
  {
    id2listener.erase(pos);
  }
}

void Signal::call(EventPtr event)
{
  map<uint32_t, ListenerPtr>::iterator i;
  for(i = id2listener.begin(); i!= id2listener.end(); ++i)
  {
    i->second->call(event);
  }
}

}
}