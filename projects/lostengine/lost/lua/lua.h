#ifndef LOST_LUA_LUA_H
#define LOST_LUA_LUA_H

extern "C" {
  #include "lua.h"
  #include "lauxlib.h"
  #include "lualib.h"
}

#include <luabind/luabind.hpp>
#include "lost/platform/Platform.h"

// from here: http://www.codeproject.com/KB/graphics/luabindLuaAndOgre3d.aspx
#define LOST_LUA_CONST_START( class, state ) { object g = globals(state); object table = g[#class];
#define LOST_LUA_CONST( class, name ) table[#name] = class::name;
#define LOST_LUA_CONST_END }

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

#endif
