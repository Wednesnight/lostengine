#ifndef LOST_LUA_LUA_H
#define LOST_LUA_LUA_H

extern "C" {
  #include "lua.h"
  #include "lauxlib.h"
  #include "lualib.h"
}

#include <luabind/luabind.hpp>
#include "lost/platform/Platform.h"

#undef LUA_NUMBER
#undef LUA_NUMBER_SCAN
#undef LUA_NUMBER_FMT
#if TARGET_OS_IPHONE
  #define LUA_NUMBER        float
  #define LUA_NUMBER_SCAN		"%f"
  #define LUA_NUMBER_FMT		"%g"
#else
  #define LUA_NUMBER        double
  #define LUA_NUMBER_SCAN		"%lf"
  #define LUA_NUMBER_FMT		"%.14g"
#endif

template <typename T>
inline T& operator << (T& other, const luabind::object& object)
{
  other = luabind::object_cast<T>(object);
  return other;
}

#endif
