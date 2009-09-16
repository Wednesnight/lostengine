#ifndef LOST_LUA_LUAEVENTCAST_H
#define LOST_LUA_LUAEVENTCAST_H

#include "lost/event/Event.h"

namespace lost
{
  namespace lua
  {
    template <class Class>
    lost::shared_ptr<Class> cast(lost::shared_ptr<lost::event::Event>& inEvent)
    {
      return lost::dynamic_pointer_cast<Class>(inEvent);
    }
  }
}

#endif
