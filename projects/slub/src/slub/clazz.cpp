/*
The MIT License (MIT)
Copyright (c) 2011 Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "../../include/slub/config.h"
#include "../../include/slub/clazz.h"

#include <iostream>
#include <stdexcept>

namespace slub {

  // TODO: access by type
  int abstract_clazz::index(lua_State* L) {
    registry* reg = registry::get(*((wrapper_base*) lua_touserdata(L, 1))->type);
    if (reg != NULL) {
      const char* name = lua_tostring(L, -1);
      if (reg->containsField(name)) {
        return reg->getField(lua_touserdata(L, 1), name)->get(L);
      }
      else if (reg->containsMethod(name)) {
        lua_pushvalue(L, -1);
        lua_pushcclosure(L, callMethod, 1);
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
    return 0;
  }
  
  int abstract_clazz::newindex(lua_State* L) {
    registry* reg = registry::get(*((wrapper_base*) lua_touserdata(L, 1))->type);
    if (reg != NULL) {
      string name(lua_tostring(L, -2));
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
    return 0;
  }
  
  int abstract_clazz::callMethod(lua_State* L) {
    registry* reg = registry::get(*((wrapper_base*) lua_touserdata(L, 1))->type);
    int num = lua_gettop(L);
    reg->getMethod(lua_tostring(L, lua_upvalueindex(1)), L)->call(L);
    return lua_gettop(L) - num;
  }
  
  int abstract_clazz::callOperator(lua_State* L) {
    registry* reg = registry::get(*((wrapper_base*) lua_touserdata(L, 1))->type);
    if (reg != NULL) {
      int num = lua_gettop(L);
      reg->getOperator(lua_tostring(L, lua_upvalueindex(2)), L)->op(L);
      return lua_gettop(L) - num;
    }
    return 0;
  }
  
}
