#include "lost/event/Handle.h"

namespace lost
{
namespace event
{

Handle::Handle()
{
  pool = NULL;
  event = NULL;
}

Handle::Handle(const Handle& other)
{
  pool = other.pool;
  event = other.event;
}

Handle::~Handle()
{
}

}
}