#ifndef SLUB_CONVERTER_H
#define SLUB_CONVERTER_H

#include <slub/registry.h>
#include <slub/wrapper.h>
#include <stdexcept>
#include <iostream>

namespace slub {

  struct empty{};

  template<typename T>
  struct converter {

    static bool check(lua_State* L, int index) {
      void *p = lua_touserdata(L, index);
      if (p != NULL) {  /* value is a userdata? */
        if (lua_getmetatable(L, index)) {  /* does it have a metatable? */
          lua_getfield(L, LUA_REGISTRYINDEX, registry<T*>::getTypeName().c_str());  /* get correct metatable */
          if (lua_rawequal(L, -1, -2)) {  /* does it have the correct mt? */
            lua_pop(L, 2);  /* remove both metatables */
            return true;
          }
        }
      }
      return false;
    }

    static T get(lua_State* L, int index) {
      if (registry<T*>::isRegisteredType()) {
        std::cout << "get, registered" << std::endl;
        wrapper<T*>* w = static_cast<wrapper<T*>*>(luaL_checkudata(L, index, registry<T*>::getTypeName().c_str()));
        return *w->ref;
      }
      throw std::runtime_error("trying to use unregistered type");
    }

    static int push(lua_State* L, T value) {
      if (registry<T*>::isRegisteredType()) {
        std::cout << "push, registered" << std::endl;
        wrapper<T*>* w = (wrapper<T*>*) lua_newuserdata(L, sizeof(wrapper<T*>));
        w->ref = new T(value);
        w->gc = true;
        luaL_getmetatable(L, registry<T*>::getTypeName().c_str());
        lua_setmetatable(L, -2);
        return 1;
      }
      throw std::runtime_error("trying to use unregistered type");
    }

  };

  template<typename T>
  struct converter<T*> {
    
    static bool check(lua_State* L, int index) {
      return converter<T>::check(L, index);
    }

    static T* get(lua_State* L, int index) {
      if (registry<T*>::isRegisteredType()) {
        std::cout << "get, registered" << std::endl;
        wrapper<T*>* w = static_cast<wrapper<T*>*>(luaL_checkudata(L, index, registry<T*>::getTypeName().c_str()));
        return w->ref;
      }
      throw std::runtime_error("trying to use unregistered type");
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
        return 1;
      }
      throw std::runtime_error("trying to use unregistered type");
    }
    
  };
  
  template<typename T>
  struct converter<const T*> {
    
    static bool check(lua_State* L, int index) {
      return converter<T>::check(L, index);
    }
    
    static const T* get(lua_State* L, int index) {
      if (registry<T*>::isRegisteredType()) {
        std::cout << "get, registered" << std::endl;
        wrapper<const T*>* w = static_cast<wrapper<const T*>*>(luaL_checkudata(L, index, registry<T*>::getTypeName().c_str()));
        return w->ref;
      }
      throw std::runtime_error("trying to use unregistered type");
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
        return 1;
      }
      throw std::runtime_error("trying to use unregistered type");
    }
    
  };
  
  template<typename T>
  struct converter<T&> {
    
    static bool check(lua_State* L, int index) {
      return converter<T>::check(L, index);
    }
    
    static T& get(lua_State* L, int index) {
      if (registry<T*>::isRegisteredType()) {
        std::cout << "get, registered" << std::endl;
        wrapper<T*>* w = static_cast<wrapper<T*>*>(luaL_checkudata(L, index, registry<T*>::getTypeName().c_str()));
        return *w->ref;
      }
      throw std::runtime_error("trying to use unregistered type");
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
        return 1;
      }
      throw std::runtime_error("trying to use unregistered type");
    }
    
  };
  
  template<typename T>
  struct converter<const T&> {
    
    static bool check(lua_State* L, int index) {
      return converter<T>::check(L, index);
    }
    
    static const T& get(lua_State* L, int index) {
      if (registry<T*>::isRegisteredType()) {
        std::cout << "get, registered" << std::endl;
        wrapper<const T*>* w = static_cast<wrapper<const T*>*>(luaL_checkudata(L, index, registry<T*>::getTypeName().c_str()));
        return *w->ref;
      }
      throw std::runtime_error("trying to use unregistered type");
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
        return 1;
      }
      return 0;
    }
    
  };
  
  template<>
  struct converter<bool> {
    
    static bool check(lua_State* L, int index) {
      return lua_isboolean(L, index);
    }
    
    static bool get(lua_State* L, int index) {
      luaL_checktype(L, index, LUA_TBOOLEAN);
      return lua_toboolean(L, index);
    }
    
    static int push(lua_State* L, bool value) {
      lua_pushboolean(L, value);
      return 1;
    }
    
  };
  
  template<>
  struct converter<int> {

    static bool check(lua_State* L, int index) {
      return lua_isnumber(L, index);
    }
    
    static int get(lua_State* L, int index) {
      return luaL_checkinteger(L, index);
    }

    static int push(lua_State* L, int value) {
      lua_pushinteger(L, value);
      return 1;
    }

  };

  template<>
  struct converter<float> {
    
    static bool check(lua_State* L, int index) {
      return lua_isnumber(L, index);
    }
    
    static float get(lua_State* L, int index) {
      return luaL_checknumber(L, index);
    }
    
    static int push(lua_State* L, float value) {
      lua_pushnumber(L, value);
      return 1;
    }
    
  };
  
  template<>
  struct converter<double> {
    
    static bool check(lua_State* L, int index) {
      return lua_isnumber(L, index);
    }
    
    static float get(lua_State* L, int index) {
      return luaL_checknumber(L, index);
    }
    
    static int push(lua_State* L, double value) {
      lua_pushnumber(L, value);
      return 1;
    }
    
  };
  
  template<>
  struct converter<std::string> {
    
    static bool check(lua_State* L, int index) {
      return lua_isstring(L, index);
    }
    
    static std::string get(lua_State* L, int index) {
      return luaL_checkstring(L, index);
    }
    
    static int push(lua_State* L, std::string value) {
      lua_pushstring(L, value.c_str());
      return 1;
    }
    
  };
  
  template<>
  struct converter<const std::string&> {
    
    static bool check(lua_State* L, int index) {
      return lua_isstring(L, index);
    }
    
    static std::string get(lua_State* L, int index) {
      return luaL_checkstring(L, index);
    }
    
    static int push(lua_State* L, const std::string& value) {
      lua_pushstring(L, value.c_str());
      return 1;
    }
    
  };
  
  template<>
  struct converter<std::string&> {
    
    static bool check(lua_State* L, int index) {
      return lua_isstring(L, index);
    }
    
    static std::string get(lua_State* L, int index) {
      return luaL_checkstring(L, index);
    }
    
    static int push(lua_State* L, std::string& value) {
      lua_pushstring(L, value.c_str());
      return 1;
    }
    
  };
  
  template<>
  struct converter<void*> {
    
    static bool check(lua_State* L, int index) {
      return lua_isuserdata(L, index);
    }
    
    static void* get(lua_State* L, int index) {
      luaL_checktype(L, index, LUA_TLIGHTUSERDATA);
      return lua_touserdata(L, index);
    }
    
    static int push(lua_State* L, void* value) {
      lua_pushlightuserdata(L, value);
      return 1;
    }
    
  };
  
}

#endif
