#ifndef LOST_LUA_LUABINDINGS_H
#define LOST_LUA_LUABINDINGS_H

#include "lost/forward/lost/lua"

namespace lost
{
  namespace lua
  {
    void bindAll(lua_State* state);
  }
}

#endif
