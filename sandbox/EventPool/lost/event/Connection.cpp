#include "lost/event/Connection.h"

namespace lost
{
namespace event
{

Connection::Connection()
{
  index = 0;
}

Connection::Connection(const string& t, uint32_t idx)
{
  eventType = t;
  index = idx;
}


}
}