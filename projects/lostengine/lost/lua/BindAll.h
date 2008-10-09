#ifndef LOST_LUA_LUABINDINGS_H
#define LOST_LUA_LUABINDINGS_H

#include "lost/forward/lost/lua/State.h"

namespace lost
{
  namespace lua
  {
    void bindAll(lost::lua::State& state);
  }
}

#endif
