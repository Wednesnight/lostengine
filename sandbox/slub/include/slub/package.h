#ifndef SLUB_PACKAGE_H
#define SLUB_PACKAGE_H

#include <slub/clazz.h>

namespace slub {

  struct package_ {

    lua_State* state;
    int table;

    package_(lua_State* L, const std::string& name, int target = -1) : state(L) {
      lua_pushstring(L, name.c_str());
      lua_gettable(L, target != -1 ? target : LUA_GLOBALSINDEX);
      table = lua_gettop(L);
      if (lua_type(L, table) == LUA_TNIL) {
        std::cout << "creating package: " << name << std::endl;
        lua_newtable(L);
        table = lua_gettop(L);

        lua_pushstring(L, name.c_str());
        lua_pushvalue(L, table);
        lua_settable(L, target != -1 ? target : LUA_GLOBALSINDEX);
      }
      else {
        std::cout << "using package: " << name << std::endl;
      }
    }

    package_ package(const std::string& name) {
      return package_(state, name, table);
    }

    template<typename T>
    slub::clazz<T> clazz(const std::string& name) {
      return slub::clazz<T>(state, name, table);
    }

    void function(const std::string& name, void (*f)()) {
      slub::function(state, name, f, table);
    }
    
    template<typename arg1>
    void function(const std::string& name, void (*f)(arg1)) {
      slub::function<arg1>(state, name, f, table);
    }
    
    template<typename arg1, typename arg2>
    void function(const std::string& name, void (*f)(arg1, arg2)) {
      slub::function<arg1, arg2>(state, name, f, table);
    }
    
    template<typename arg1, typename arg2, typename arg3>
    void function(const std::string& name, void (*f)(arg1, arg2, arg3)) {
      slub::function<arg1, arg2, arg3>(state, name, f, table);
    }
    
    template<typename R>
    void function(const std::string& name, R (*f)()) {
      slub::function<R>(state, name, f, table);
    }
    
    template<typename R, typename arg1>
    void function(const std::string& name, R (*f)(arg1)) {
      slub::function<R, arg1>(state, name, f, table);
    }
    
    template<typename R, typename arg1, typename arg2>
    void function(const std::string& name, R (*f)(arg1, arg2)) {
      slub::function<R, arg1, arg2>(state, name, f, table);
    }
    
    template<typename R, typename arg1, typename arg2, typename arg3>
    void function(const std::string& name, R (*f)(arg1, arg2, arg3)) {
      slub::function<R, arg1, arg2, arg3>(state, name, f, table);
    }
    
  };

  typedef package_ package;

}

#endif
