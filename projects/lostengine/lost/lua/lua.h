#ifndef LOST_LUA_LUA_H
#define LOST_LUA_LUA_H

#include <slub/slub.h>
#include "lost/platform/Type.h"

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

#ifdef LOST_USE_EASTL
namespace slub {
  
  template<>
  struct converter<lost::string> {
    
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
  
  template<>
  struct converter<const lost::string&> : converter<lost::string> {};
  
  template<>
  struct converter<lost::string*> {
    
    static bool check(lua_State* L, int index) {
      return lua_isstring(L, index);
    }
    
    static lost::string get(lua_State* L, int index) {
      return luaL_checkstring(L, index);
    }
    
    static int push(lua_State* L, const lost::string* value) {
      lua_pushstring(L, value->c_str());
      return 1;
    }
    
  };

  template<>
  struct converter<const lost::string*> : converter<lost::string*> {};
  
}


#endif

#endif
