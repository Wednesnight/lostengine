#ifndef LOST_LUA_LUAEVENTCAST_H
#define LOST_LUA_LUAEVENTCAST_H

#include <boost/shared_ptr.hpp>
#include "lost/event/Event.h"
#include "lost/common/Logger.h"

namespace lost
{
  namespace lua
  {
    template <class Class>
    boost::shared_ptr<Class> cast(boost::shared_ptr<lost::event::Event>& inEvent)
    {
      DOUT("lost::lua::cast()");
      return boost::dynamic_pointer_cast<Class>(inEvent);
    }
  }
}

#endif
