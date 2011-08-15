#include <slub/clazz.h>

#include <iostream>
#include <stdexcept>

namespace slub {

  int abstract_clazz::__index(lua_State* L) {
    registry* reg = (registry*) lua_touserdata(L, lua_upvalueindex(1));
    std::string name(lua_tostring(L, -1));
    
    if (reg->containsField(name)) {
      return reg->getField(lua_touserdata(L, 1), name)->get(L);
    }
    else if (reg->containsMethod(name)) {
      lua_pushlightuserdata(L, reg);
      lua_pushvalue(L, -2);
      lua_pushcclosure(L, __callMethod, 2);
      return 1;
    }
    else {
      // get value from Lua table
      luaL_getmetatable(L, reg->getTypeName().c_str());
      lua_getfield(L, -1, "__metatable");
      int methods = lua_gettop(L);
    
      lua_pushvalue(L, -3);
      lua_gettable(L, methods);
      
      return 1;
    }
  }
  
  int abstract_clazz::__newindex(lua_State* L) {
    registry* reg = (registry*) lua_touserdata(L, lua_upvalueindex(1));
    std::string name(lua_tostring(L, -2));

    if (reg->containsField(name)) {
      return reg->getField(lua_touserdata(L, 1), name)->set(L);
    }
    else {
      luaL_getmetatable(L, reg->getTypeName().c_str());
      lua_getfield(L, -1, "__metatable");
      int methods = lua_gettop(L);
      
      lua_pushvalue(L, -3);
      lua_setfield(L, methods, name.c_str());
      
      lua_pop(L, 2);
      return 0;
    }
  }
  
  int abstract_clazz::__callMethod(lua_State* L) {
    registry* reg = (registry*) lua_touserdata(L, lua_upvalueindex(1));
    int num = lua_gettop(L);
    reg->getMethod(lua_tostring(L, lua_upvalueindex(2)), L)->call(L);
    return lua_gettop(L) - num;
  }
  
  int abstract_clazz::__callOperator(lua_State* L) {
    registry* reg = (registry*) lua_touserdata(L, lua_upvalueindex(1));
    int num = lua_gettop(L);
    reg->getOperator(lua_tostring(L, lua_upvalueindex(2)), L)->op(L);
    return lua_gettop(L) - num;
  }
  
}
