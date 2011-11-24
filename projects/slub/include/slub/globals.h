#ifndef SLUB_GLOBALS_H
#define SLUB_GLOBALS_H

#include "slub_lua.h"
#include "reference.h"

namespace slub {

  struct globals {
    
    globals(lua_State* state) : state(state) {
    }
    
    globals(const globals& g) {
      *this = g;
    }
    
    void operator=(const globals& g) {
      this->state = g.state;
    }
    
    int type() const {
      return lua_type(state, LUA_GLOBALSINDEX);
    }
    
    std::string typeName() const {
      return lua_typename(state, type());
    }
    
    reference operator[](const std::string& name) const {
      lua_getglobal(state, name.c_str());
      reference result(state);
      result.name = name;
      return result;
    }
    
  protected:
    
    lua_State* state;
    
  };
  
}

#endif
