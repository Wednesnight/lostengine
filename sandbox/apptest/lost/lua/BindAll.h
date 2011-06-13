#ifndef LOST_LUA_LUABINDINGS_H
#define LOST_LUA_LUABINDINGS_H

#include "lost/lua/forward.h"

namespace lost
{
  namespace lua
  {
    void bindAll(lua_State* state);
  }
}

#endif
