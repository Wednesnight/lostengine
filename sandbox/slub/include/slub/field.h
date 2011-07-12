#ifndef SLUB_FIELD_H
#define SLUB_FIELD_H

#include <slub/converter.h>
#include <slub/wrapper.h>

namespace slub {

  struct abstract_field {
    virtual int get(lua_State*) = 0;
    virtual int set(lua_State*) = 0;
  };

  template<typename T, typename F>
  struct field : public abstract_field {

    F T::*m;

    field(F T::*m) : m(m) {
    }

    int get(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(lua_touserdata(L, 1));
      converter<F>::push(L, t->ref->*m);
      return 1;
    }

    int set(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(lua_touserdata(L, 1));
      t->ref->*m = converter<F>::get(L, -1);
      return 0;
    }

  };

  template<typename T>
  struct field<T, int> : public abstract_field {

    int T::*m;
    
    field(int T::*m) : m(m) {
    }
    
    int get(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(lua_touserdata(L, 1));
      lua_pushinteger(L, t->ref->*m);
      return 1;
    }

    int set(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(lua_touserdata(L, 1));
      t->ref->*m = lua_tointeger(L, -1);
      return 0;
    }

  };

  template<typename T>
  struct field<T, float> : public abstract_field {
    
    float T::*m;
    
    field(float T::*m) : m(m) {
    }
    
    int get(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(lua_touserdata(L, 1));
      lua_pushnumber(L, t->ref->*m);
      return 1;
    }
    
    int set(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(lua_touserdata(L, 1));
      t->ref->*m = lua_tonumber(L, -1);
      return 0;
    }
    
  };
  
}

#endif
