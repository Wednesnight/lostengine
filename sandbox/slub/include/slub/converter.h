#ifndef SLUB_CONVERTER_H
#define SLUB_CONVERTER_H

#include <slub/registry.h>
#include <slub/wrapper.h>
#include <stdexcept>
#include <iostream>

namespace slub {

  template<typename T>
  struct converter {

    static T get(lua_State* L, int index) {
      if (registry<T>::isRegisteredType()) {
        std::cout << "get, registered" << std::endl;
        wrapper<T>* w = static_cast<wrapper<T>*>(lua_touserdata(L, index));
        return w->ref;
      }
      else {
        std::cout << "get, unregistered" << std::endl;
        return static_cast<T>(lua_touserdata(L, index));
      }
    }

    static int push(lua_State* L, T value) {
      if (registry<T>::isRegisteredType()) {
        std::cout << "push, registered" << std::endl;
        wrapper<T>* w = (wrapper<T>*) lua_newuserdata(L, sizeof(wrapper<T>));
        w->ref = value;
        w->gc = false;
        luaL_getmetatable(L, registry<T>::getTypeName().c_str());
        lua_setmetatable(L, -2);
      }
      else {
        std::cout << "push, unregistered" << std::endl;
        lua_pushlightuserdata(L, value);
      }
      return 1;
    }

  };

  template<>
  struct converter<int> {

    static int get(lua_State* L, int index) {
      return lua_tonumber(L, index);
    }

    static int push(lua_State* L, int value) {
      lua_pushnumber(L, value);
      return 1;
    }

  };

  template<>
  struct converter<float> {
    
    static int get(lua_State* L, int index) {
      return lua_tonumber(L, index);
    }
    
    static int push(lua_State* L, int value) {
      lua_pushnumber(L, value);
      return 1;
    }
    
  };
  
}

#endif
