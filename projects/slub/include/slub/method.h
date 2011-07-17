#ifndef SLUB_METHOD_H
#define SLUB_METHOD_H

#include <slub/converter.h>
#include <slub/wrapper.h>

namespace slub {

  struct abstract_method {
    virtual bool check(lua_State*) = 0;
    virtual int call(lua_State*) = 0;
  };

  template<typename T, typename ret = void, typename arg1 = empty, typename arg2 = empty>
  struct method : public abstract_method {
    
    ret (T::*m)(arg1, arg2);
    
    method(ret (T::*m)(arg1, arg2)) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 3 && converter<arg1>::check(L, -2) && converter<arg2>::check(L, -1);
    }

    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(luaL_checkudata(L, 1, registry<T*>::getTypeName().c_str()));
      return converter<ret>::push(L, (t->ref->*m)(converter<arg1>::get(L, -2), converter<arg2>::get(L, -1)));
    }
    
  };
  
  template<typename T>
  struct method<T, void, empty, empty> : public abstract_method {
    
    void (T::*m)();
    
    method(void (T::*m)()) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 1;
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(luaL_checkudata(L, 1, registry<T*>::getTypeName().c_str()));
      (t->ref->*m)();
      return 0;
    }
    
  };
  
  template<typename T, typename ret>
  struct method<T, ret, empty, empty> : public abstract_method {
    
    ret (T::*m)();
    
    method(ret (T::*m)()) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 1;
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(luaL_checkudata(L, 1, registry<T*>::getTypeName().c_str()));
      return converter<ret>::push(L, (t->ref->*m)());
    }
    
  };
  
  template<typename T, typename arg1>
  struct method<T, void, arg1, empty> : public abstract_method {
    
    void (T::*m)(arg1);
    
    method(void (T::*m)(arg1)) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 2 && converter<arg1>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(luaL_checkudata(L, 1, registry<T*>::getTypeName().c_str()));
      (t->ref->*m)(converter<arg1>::get(L, -1));
      return 0;
    }
    
  };
  
  template<typename T, typename ret, typename arg1>
  struct method<T, ret, arg1, empty> : public abstract_method {
    
    ret (T::*m)(arg1);
    
    method(ret (T::*m)(arg1)) : m(m) {
    }

    bool check(lua_State* L) {
      return lua_gettop(L) == 2 && converter<arg1>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(luaL_checkudata(L, 1, registry<T*>::getTypeName().c_str()));
      return converter<ret>::push(L, (t->ref->*m)(converter<arg1>::get(L, -1)));
    }
    
  };
  
  template<typename T, typename arg1, typename arg2>
  struct method<T, void, arg1, arg2> : public abstract_method {
    
    void (T::*m)(arg1, arg2);
    
    method(void (T::*m)(arg1, arg2)) : m(m) {
    }

    bool check(lua_State* L) {
      return lua_gettop(L) == 3 && converter<arg1>::check(L, -2) && converter<arg2>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(luaL_checkudata(L, 1, registry<T*>::getTypeName().c_str()));
      (t->ref->*m)(converter<arg1>::get(L, -2), converter<arg2>::get(L, -1));
      return 0;
    }
    
  };

}

#endif
