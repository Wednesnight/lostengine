#ifndef LOST_EVENT_CONNECTION_H
#define LOST_EVENT_CONNECTION_H

#include "lost/event/EventType.h"

namespace lost
{
namespace event
{
struct Connection
{
  Connection();  
  Connection(const event::Type& t, uint32_t idx);
  event::Type eventType;
  uint32_t      index;
};
}
}

#endif