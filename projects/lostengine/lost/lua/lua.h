#ifndef LOST_LUA_LUA_H
#define LOST_LUA_LUA_H

extern "C" {
  #include "lua.h"
  #include "lauxlib.h"
  #include "lualib.h"
}

#include <luabind/luabind.hpp>
#include "lost/platform/Type.h"

#include <slub/slub.h>

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

#ifdef LOST_USE_EASTL
namespace luabind
{
template <>
struct default_converter<lost::string>
  : native_converter_base<lost::string>
{
    static int compute_score(lua_State* L, int index)
    {
        return lua_type(L, index) == LUA_TSTRING ? 0 : -1;
    }

    lost::string from(lua_State* L, int index)
    {
        return lost::string(lua_tostring(L, index), lua_strlen(L, index));
    }

    void to(lua_State* L, lost::string const& value)
    {
        lua_pushlstring(L, value.data(), value.size());
    }
};

template <>
struct default_converter<lost::string const>
  : default_converter<lost::string>
{};

template <>
struct default_converter<lost::string const&>
  : default_converter<lost::string>
{};
}

namespace slub {
  
  template<>
  struct converter<lost::string> {
    
    static bool check(lua_State* L, int index) {
      return lua_isstring(L, index);
    }
    
    static lost::string get(lua_State* L, int index) {
      return luaL_checkstring(L, index);
    }
    
    static int push(lua_State* L, lost::string value) {
      lua_pushstring(L, value.c_str());
      return 1;
    }
    
  };
  
  template<>
  struct converter<const lost::string&> {
    
    static bool check(lua_State* L, int index) {
      return lua_isstring(L, index);
    }
    
    static lost::string get(lua_State* L, int index) {
      return luaL_checkstring(L, index);
    }
    
    static int push(lua_State* L, const lost::string& value) {
      lua_pushstring(L, value.c_str());
      return 1;
    }
    
  };
  
}


#endif

#endif
