#ifndef LOST_EVENT_SIGNAL_H
#define LOST_EVENT_SIGNAL_H

#include "lost/event/forward.h"

namespace lost
{
namespace event
{

struct Signal
{
  Signal();
  virtual ~Signal();
  
  void addListener();
  
  void call(EventPtr event);
  
  
};

}
}

#endif
