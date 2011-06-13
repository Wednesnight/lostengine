#ifndef LOST_EVENT_EVENT_H
#define LOST_EVENT_EVENT_H

namespace lost
{
namespace event
{
struct Event
{
  Event(); 
  virtual ~Event();
  
  uint32_t val;
};
}
}
#endif

