#ifndef LOST_LUA_ERRORHANDLER_H
#define LOST_LUA_ERRORHANDLER_H

#include "lost/forward/lost/lua/lua.h"

namespace lost
{
  namespace lua
  {
    int errorHandler(lua_State* state);
  }
}

#endif
