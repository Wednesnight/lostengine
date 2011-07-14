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
      if (registry<T*>::isRegisteredType()) {
        std::cout << "get, registered" << std::endl;
        wrapper<T*>* w = static_cast<wrapper<T*>*>(lua_touserdata(L, index));
        return *w->ref;
      }
      else {
        std::cout << "get, unregistered" << std::endl;
        return *static_cast<T*>(lua_touserdata(L, index));
      }
    }

    static int push(lua_State* L, T value) {
      return push(L, value, false);
    }

    static int push(lua_State* L, T value, bool gc) {
      if (registry<T*>::isRegisteredType()) {
        std::cout << "push, registered" << std::endl;
        wrapper<T*>* w = (wrapper<T*>*) lua_newuserdata(L, sizeof(wrapper<T*>));
        w->ref = &value;
        w->gc = gc;
        luaL_getmetatable(L, registry<T*>::getTypeName().c_str());
        lua_setmetatable(L, -2);
      }
      else {
        std::cout << "push, unregistered" << std::endl;
        lua_pushlightuserdata(L, &value);
      }
      return 1;
    }

  };

  template<typename T>
  struct converter<T*> {
    
    static T* get(lua_State* L, int index) {
      if (registry<T*>::isRegisteredType()) {
        std::cout << "get, registered" << std::endl;
        wrapper<T*>* w = static_cast<wrapper<T*>*>(lua_touserdata(L, index));
        return w->ref;
      }
      else {
        std::cout << "get, unregistered" << std::endl;
        return static_cast<T*>(lua_touserdata(L, index));
      }
    }
    
    static int push(lua_State* L, T* value) {
      return push(L, value, false);
    }
    
    static int push(lua_State* L, T* value, bool gc) {
      if (registry<T*>::isRegisteredType()) {
        std::cout << "push, registered" << std::endl;
        wrapper<T*>* w = (wrapper<T*>*) lua_newuserdata(L, sizeof(wrapper<T*>));
        w->ref = value;
        w->gc = gc;
        luaL_getmetatable(L, registry<T*>::getTypeName().c_str());
        lua_setmetatable(L, -2);
      }
      else {
        std::cout << "push, unregistered" << std::endl;
        lua_pushlightuserdata(L, value);
      }
      return 1;
    }
    
  };
  
  template<typename T>
  struct converter<const T*> {
    
    static const T* get(lua_State* L, int index) {
      if (registry<T*>::isRegisteredType()) {
        std::cout << "get, registered" << std::endl;
        wrapper<const T*>* w = static_cast<wrapper<const T*>*>(lua_touserdata(L, index));
        return w->ref;
      }
      else {
        std::cout << "get, unregistered" << std::endl;
        return static_cast<const T*>(lua_touserdata(L, index));
      }
    }
    
    static int push(lua_State* L, const T* value) {
      return push(L, value, false);
    }
    
    static int push(lua_State* L, const T* value, bool gc) {
      if (registry<T*>::isRegisteredType()) {
        std::cout << "push, registered" << std::endl;
        wrapper<const T*>* w = (wrapper<const T*>*) lua_newuserdata(L, sizeof(wrapper<const T*>));
        w->ref = value;
        w->gc = gc;
        luaL_getmetatable(L, registry<T*>::getTypeName().c_str());
        lua_setmetatable(L, -2);
      }
      else {
        std::cout << "push, unregistered" << std::endl;
        lua_pushlightuserdata(L, value);
      }
      return 1;
    }
    
  };
  
  template<typename T>
  struct converter<T&> {
    
    static T& get(lua_State* L, int index) {
      if (registry<T*>::isRegisteredType()) {
        std::cout << "get, registered" << std::endl;
        wrapper<T*>* w = static_cast<wrapper<T*>*>(lua_touserdata(L, index));
        return *w->ref;
      }
      else {
        std::cout << "get, unregistered" << std::endl;
        return *static_cast<T*>(lua_touserdata(L, index));
      }
    }
    
    static int push(lua_State* L, T& value) {
      return push(L, value, false);
    }
    
    static int push(lua_State* L, T& value, bool gc) {
      if (registry<T*>::isRegisteredType()) {
        std::cout << "push, registered" << std::endl;
        wrapper<T*>* w = (wrapper<T*>*) lua_newuserdata(L, sizeof(wrapper<T*>));
        w->ref = &value;
        w->gc = gc;
        luaL_getmetatable(L, registry<T*>::getTypeName().c_str());
        lua_setmetatable(L, -2);
      }
      else {
        std::cout << "push, unregistered" << std::endl;
        lua_pushlightuserdata(L, &value);
      }
      return 1;
    }
    
  };
  
  template<typename T>
  struct converter<const T&> {
    
    static const T& get(lua_State* L, int index) {
      if (registry<T*>::isRegisteredType()) {
        std::cout << "get, registered" << std::endl;
        wrapper<const T*>* w = static_cast<wrapper<const T*>*>(lua_touserdata(L, index));
        return *w->ref;
      }
      else {
        std::cout << "get, unregistered" << std::endl;
        return *static_cast<const T*>(lua_touserdata(L, index));
      }
    }
    
    static int push(lua_State* L, const T& value) {
      return push(L, value, false);
    }
    
    static int push(lua_State* L, const T& value, bool gc) {
      if (registry<T*>::isRegisteredType()) {
        std::cout << "push, registered" << std::endl;
        wrapper<const T*>* w = (wrapper<const T*>*) lua_newuserdata(L, sizeof(wrapper<const T*>));
        w->ref = &value;
        w->gc = gc;
        luaL_getmetatable(L, registry<T*>::getTypeName().c_str());
        lua_setmetatable(L, -2);
      }
      else {
        std::cout << "push, unregistered" << std::endl;
        lua_pushlightuserdata(L, &value);
      }
      return 1;
    }
    
  };
  
  template<>
  struct converter<bool> {
    
    static bool get(lua_State* L, int index) {
      return lua_toboolean(L, index);
    }
    
    static int push(lua_State* L, bool value) {
      lua_pushboolean(L, value);
      return 1;
    }
    
  };
  
  template<>
  struct converter<int> {

    static int get(lua_State* L, int index) {
      return lua_tointeger(L, index);
    }

    static int push(lua_State* L, int value) {
      lua_pushinteger(L, value);
      return 1;
    }

  };

  template<>
  struct converter<float> {
    
    static float get(lua_State* L, int index) {
      return lua_tonumber(L, index);
    }
    
    static int push(lua_State* L, float value) {
      lua_pushnumber(L, value);
      return 1;
    }
    
  };
  
}

#endif
