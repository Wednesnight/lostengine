#ifndef L_EVENT_H
#define L_EVENT_H

#include <stdint.h>

struct Event
{
  Event(); 
  virtual ~Event();
  
  uint32_t val;
};

#endif
