#ifndef LOST_LUA_LUA_H
#define LOST_LUA_LUA_H

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <luabind/luabind.hpp>

// from here: http://www.codeproject.com/KB/graphics/luabindLuaAndOgre3d.aspx
#define LOST_LUA_CONST_START( class, state ) { object g = globals(state); object table = g[#class];
#define LOST_LUA_CONST( class, name ) table[#name] = class::name;
#define LOST_LUA_CONST_END }

#endif
