#ifndef SLUB_PACKAGE_H
#define SLUB_PACKAGE_H

#include <slub/clazz.h>

namespace slub {

  struct package_ {

    lua_State* state;
    int table;

    package_(lua_State* L, const std::string& name, int target = -1) : state(L) {
      lua_newtable(L);
      table = lua_gettop(L);

      lua_pushstring(L, name.c_str());
      lua_pushvalue(L, table);
      lua_settable(L, target != -1 ? target : LUA_GLOBALSINDEX);      
    }

    package_ package(const std::string& name) {
      return package_(state, name, table);
    }

    template<typename T>
    slub::clazz<T> clazz(const std::string& name) {
      return slub::clazz<T>(state, name, table);
    }

  };

  typedef package_ package;

}

#endif
