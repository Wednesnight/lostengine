#include <slub/clazz.h>

#include <iostream>
#include <stdexcept>

namespace slub {

  int abstract_clazz::__index(lua_State* L) {
    std::string className(lua_tostring(L, lua_upvalueindex(1)));
    std::string name(lua_tostring(L, -1));
    
    if (registry::get(className)->containsField(name)) {
      return registry::get(className)->getField(name)->get(L);
    }
    else if (registry::get(className)->containsMethod(name)) {
      lua_pushstring(L, className.c_str());
      lua_pushvalue(L, -2);
      lua_pushcclosure(L, __callMethod, 2);
      return 1;
    }
    else {
      // get value from Lua table
      luaL_getmetatable(L, className.c_str());
      lua_getfield(L, -1, "__metatable");
      int methods = lua_gettop(L);
    
      lua_pushvalue(L, -3);
      lua_gettable(L, methods);
      
      return 1;
    }
  }
  
  int abstract_clazz::__newindex(lua_State* L) {
    std::string className(lua_tostring(L, lua_upvalueindex(1)));
    std::string name(lua_tostring(L, -2));
    if (registry::get(className)->containsField(name)) {
      return registry::get(className)->getField(name)->set(L);
    }
    else {
      luaL_getmetatable(L, className.c_str());
      lua_getfield(L, -1, "__metatable");
      int methods = lua_gettop(L);
      
      lua_pushvalue(L, -3);
      lua_setfield(L, methods, name.c_str());
      
      lua_pop(L, 2);
      return 0;
    }
  }
  
  int abstract_clazz::__callMethod(lua_State* L) {
    return registry::get(lua_tostring(L, lua_upvalueindex(1)))->getMethod(lua_tostring(L, lua_upvalueindex(2)), L)->call(L);
  }
  
  int abstract_clazz::__callOperator(lua_State* L) {
    return registry::get(lua_tostring(L, lua_upvalueindex(1)))->getOperator(lua_tostring(L, lua_upvalueindex(2)), L)->op(L);
  }
  
}
