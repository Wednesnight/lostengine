#ifndef SLUB_PACKAGE_H
#define SLUB_PACKAGE_H

#include <slub/clazz.h>

namespace slub {

  struct package_ {

    lua_State* state;
    int table;

    package_(lua_State* L, const std::string& name, int target = -1) : state(L) {
      lua_getfield(L, target != -1 ? target : LUA_GLOBALSINDEX, name.c_str());
      if (lua_type(L, lua_gettop(L)) == LUA_TNIL) {
        std::cout << "creating package: " << name << std::endl;
        lua_pop(L, 1);
        lua_newtable(L);
        lua_pushvalue(L, -1);
        lua_setfield(L, target != -1 ? target : LUA_GLOBALSINDEX, name.c_str());
      }
      else {
        std::cout << "using package: " << name << std::endl;
      }
      table = luaL_ref(L, LUA_REGISTRYINDEX);
    }

    package_ package(const std::string& name) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      package_ result(state, name, lua_gettop(state));
      lua_pop(state, 1);
      return result;
    }

    template<typename T>
    slub::clazz<T> clazz(const std::string& name) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::clazz<T> result(state, name, lua_gettop(state));
      lua_pop(state, 1);
      return result;
    }

    void function(const std::string& name, void (*f)()) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function(state, name, f, lua_gettop(state));
      lua_pop(state, 1);
    }
    
    template<typename arg1>
    void function(const std::string& name, void (*f)(arg1)) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<arg1>(state, name, f, lua_gettop(state));
      lua_pop(state, 1);
    }
    
    template<typename arg1, typename arg2>
    void function(const std::string& name, void (*f)(arg1, arg2)) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<arg1, arg2>(state, name, f, lua_gettop(state));
      lua_pop(state, 1);
    }
    
    template<typename arg1, typename arg2, typename arg3>
    void function(const std::string& name, void (*f)(arg1, arg2, arg3)) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<arg1, arg2, arg3>(state, name, f, lua_gettop(state));
      lua_pop(state, 1);
    }
    
    template<typename R>
    void function(const std::string& name, R (*f)()) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<R>(state, name, f, lua_gettop(state));
      lua_pop(state, 1);
    }
    
    template<typename R, typename arg1>
    void function(const std::string& name, R (*f)(arg1)) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<R, arg1>(state, name, f, lua_gettop(state));
      lua_pop(state, 1);
    }
    
    template<typename R, typename arg1, typename arg2>
    void function(const std::string& name, R (*f)(arg1, arg2)) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<R, arg1, arg2>(state, name, f, lua_gettop(state));
      lua_pop(state, 1);
    }
    
    template<typename R, typename arg1, typename arg2, typename arg3>
    void function(const std::string& name, R (*f)(arg1, arg2, arg3)) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<R, arg1, arg2, arg3>(state, name, f, lua_gettop(state));
      lua_pop(state, 1);
    }
    
  };

  typedef package_ package;

}

#endif
