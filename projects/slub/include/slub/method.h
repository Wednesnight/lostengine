#ifndef SLUB_METHOD_H
#define SLUB_METHOD_H

#include <slub/converter.h>
#include <slub/wrapper.h>

namespace slub {

  struct abstract_method {
    virtual int call(lua_State*) = 0;
  };

  struct empty{};

  template<typename T, typename ret = void, typename arg1 = empty, typename arg2 = empty>
  struct method : public abstract_method {
    
    ret (T::*m)(arg1, arg2);
    
    method(ret (T::*m)(arg1, arg2)) : m(m) {
    }
    
    ret operator()(T* t, arg1 a1, arg2 a2) {
      return (t->*m)(a1, a2);
    }
    
    int call(lua_State* L) {
      lua_pushlightuserdata(L, this);
      lua_pushcclosure(L, _call, 1);
      return 1;
    }
    
    static int _call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(lua_touserdata(L, 1));
      method* m = static_cast<method*>(lua_touserdata(L, lua_upvalueindex(1)));
      return converter<ret>::push(L, (*m)(t->ref, converter<arg1>::get(L, -1), converter<arg2>::get(L, -2)));
    }
    
  };
  
  template<typename T>
  struct method<T, void, empty, empty> : public abstract_method {
    
    void (T::*m)();
    
    method(void (T::*m)()) : m(m) {
    }
    
    void operator()(T* t) {
      (t->*m)();
    }
    
    int call(lua_State* L) {
      lua_pushlightuserdata(L, this);
      lua_pushcclosure(L, _call, 1);
      return 1;
    }
    
    static int _call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(lua_touserdata(L, 1));
      method* m = static_cast<method*>(lua_touserdata(L, lua_upvalueindex(1)));
      (*m)(t->ref);
      return 0;
    }
    
  };
  
  template<typename T, typename ret>
  struct method<T, ret, empty, empty> : public abstract_method {
    
    ret (T::*m)();
    
    method(ret (T::*m)()) : m(m) {
    }
    
    ret operator()(T* t) {
      return (t->*m)();
    }
    
    int call(lua_State* L) {
      lua_pushlightuserdata(L, this);
      lua_pushcclosure(L, _call, 1);
      return 1;
    }
    
    static int _call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(lua_touserdata(L, 1));
      method* m = static_cast<method*>(lua_touserdata(L, lua_upvalueindex(1)));
      return converter<ret>::push(L, (*m)(t->ref));
    }
    
  };
  
  template<typename T, typename arg1>
  struct method<T, void, arg1, empty> : public abstract_method {
    
    void (T::*m)(arg1);
    
    method(void (T::*m)(arg1)) : m(m) {
    }
    
    void operator()(T* t, arg1 a1) {
      (t->*m)(a1);
    }
    
    int call(lua_State* L) {
      lua_pushlightuserdata(L, this);
      lua_pushcclosure(L, _call, 1);
      return 1;
    }
    
    static int _call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(lua_touserdata(L, 1));
      method* m = static_cast<method*>(lua_touserdata(L, lua_upvalueindex(1)));
      (*m)(t->ref, converter<arg1>::get(L, -1));
      return 0;
    }
    
  };
  
  template<typename T, typename ret, typename arg1>
  struct method<T, ret, arg1, empty> : public abstract_method {
    
    ret (T::*m)(arg1);
    
    method(ret (T::*m)(arg1)) : m(m) {
    }
    
    ret operator()(T* t, arg1 a1) {
      return (t->*m)(a1);
    }
    
    int call(lua_State* L) {
      lua_pushlightuserdata(L, this);
      lua_pushcclosure(L, _call, 1);
      return 1;
    }
    
    static int _call(lua_State* L) {
      std::cout << lua_gettop(L) << std::endl;
      wrapper<T*>* t = static_cast<wrapper<T*>*>(lua_touserdata(L, 1));
      method* m = static_cast<method*>(lua_touserdata(L, lua_upvalueindex(1)));
      return converter<ret>::push(L, (*m)(t->ref, converter<arg1>::get(L, -1)));
    }
    
  };
  
  template<typename T, typename arg1, typename arg2>
  struct method<T, void, arg1, arg2> : public abstract_method {
    
    void (T::*m)(arg1, arg2);
    
    method(void (T::*m)(arg1, arg2)) : m(m) {
    }

    void operator()(T* t, arg1 a1, arg2 a2) {
      (t->*m)(a1, a2);
    }

    int call(lua_State* L) {
      lua_pushlightuserdata(L, this);
      lua_pushcclosure(L, _call, 1);
      return 1;
    }
    
    static int _call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(lua_touserdata(L, 1));
      method* m = static_cast<method*>(lua_touserdata(L, lua_upvalueindex(1)));
      (*m)(t->ref, converter<arg1>::get(L, -1), converter<arg2>::get(L, -2));
      return 0;
    }
    
  };

}

#endif
