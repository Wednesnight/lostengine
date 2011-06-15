#ifndef LOST_EVENT_CONNECTION_H
#define LOST_EVENT_CONNECTION_H

namespace lost
{
namespace event
{
struct Connection
{
  Connection();  
  Connection(const string& t, uint32_t idx);
  string eventType;
  uint32_t      index;
};
}
}

#endif