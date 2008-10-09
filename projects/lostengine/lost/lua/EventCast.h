#ifndef LOST_LUA_LUAEVENTCAST_H
#define LOST_LUA_LUAEVENTCAST_H

#include "lost/event/Event.h"

namespace lost
{
  namespace lua
  {
    template <class Class>
    boost::shared_ptr<Class> cast(boost::shared_ptr<lost::event::Event>& inEvent)
    {
      return boost::dynamic_pointer_cast<Class>(inEvent);
    }
  }
}

#endif
