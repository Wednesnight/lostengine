#ifndef SLUB_METHOD_H
#define SLUB_METHOD_H

#include <slub/converter.h>
#include <slub/registry.h>
#include <slub/wrapper.h>

namespace slub {

  struct abstract_method {
    virtual bool check(lua_State*) = 0;
    virtual int call(lua_State*) = 0;
  };

  template<typename T, typename ret = void, typename arg1 = empty, typename arg2 = empty, typename arg3 = empty, typename arg4 = empty, typename arg5 = empty>
  struct method : public abstract_method {
    
    ret (T::*m)(arg1, arg2, arg3, arg4, arg5);
    
    method(ret (T::*m)(arg1, arg2, arg3, arg4, arg5)) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 6 && converter<arg1>::check(L, -5) && converter<arg2>::check(L, -4) && converter<arg3>::check(L, -3) && converter<arg4>::check(L, -2)
          && converter<arg5>::check(L, -1);
    }

    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      return converter<ret>::push(L, (t->ref->*m)(converter<arg1>::get(L, -5), converter<arg2>::get(L, -4), converter<arg3>::get(L, -3), converter<arg4>::get(L, -2),
                                                  converter<arg5>::get(L, -1)));
    }
    
  };
  
  template<typename T>
  struct method<T, void, empty, empty, empty, empty, empty> : public abstract_method {
    
    void (T::*m)();
    
    method(void (T::*m)()) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 1;
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      (t->ref->*m)();
      return 0;
    }
    
  };
  
  template<typename T, typename ret>
  struct method<T, ret, empty, empty, empty, empty, empty> : public abstract_method {
    
    ret (T::*m)();
    
    method(ret (T::*m)()) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 1;
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      return converter<ret>::push(L, (t->ref->*m)());
    }
    
  };
  
  template<typename T, typename arg1>
  struct method<T, void, arg1, empty, empty, empty, empty> : public abstract_method {
    
    void (T::*m)(arg1);
    
    method(void (T::*m)(arg1)) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 2 && converter<arg1>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      (t->ref->*m)(converter<arg1>::get(L, -1));
      return 0;
    }
    
  };
  
  template<typename T, typename ret, typename arg1>
  struct method<T, ret, arg1, empty, empty, empty, empty> : public abstract_method {
    
    ret (T::*m)(arg1);
    
    method(ret (T::*m)(arg1)) : m(m) {
    }

    bool check(lua_State* L) {
      return lua_gettop(L) == 2 && converter<arg1>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      return converter<ret>::push(L, (t->ref->*m)(converter<arg1>::get(L, -1)));
    }
    
  };
  
  template<typename T, typename arg1, typename arg2>
  struct method<T, void, arg1, arg2, empty, empty, empty> : public abstract_method {
    
    void (T::*m)(arg1, arg2);
    
    method(void (T::*m)(arg1, arg2)) : m(m) {
    }

    bool check(lua_State* L) {
      return lua_gettop(L) == 3 && converter<arg1>::check(L, -2) && converter<arg2>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      (t->ref->*m)(converter<arg1>::get(L, -2), converter<arg2>::get(L, -1));
      return 0;
    }
    
  };

  template<typename T, typename ret, typename arg1, typename arg2>
  struct method<T, ret, arg1, arg2, empty, empty, empty> : public abstract_method {
    
    ret (T::*m)(arg1, arg2);
    
    method(ret (T::*m)(arg1, arg2)) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 3 && converter<arg1>::check(L, -2) && converter<arg2>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      return converter<ret>::push(L, (t->ref->*m)(converter<arg1>::get(L, -2), converter<arg2>::get(L, -1)));
    }
    
  };
  
  template<typename T, typename arg1, typename arg2, typename arg3>
  struct method<T, void, arg1, arg2, arg3, empty, empty> : public abstract_method {
    
    void (T::*m)(arg1, arg2, arg3);
    
    method(void (T::*m)(arg1, arg2, arg3)) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 4 && converter<arg1>::check(L, -3) && converter<arg2>::check(L, -2) && converter<arg3>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      (t->ref->*m)(converter<arg1>::get(L, -3), converter<arg2>::get(L, -2), converter<arg3>::get(L, -1));
      return 0;
    }
    
  };
  
  template<typename T, typename ret, typename arg1, typename arg2, typename arg3>
  struct method<T, ret, arg1, arg2, arg3, empty, empty> : public abstract_method {
    
    ret (T::*m)(arg1, arg2, arg3);
    
    method(ret (T::*m)(arg1, arg2, arg3)) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 4 && converter<arg1>::check(L, -3) && converter<arg2>::check(L, -2) && converter<arg3>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      return converter<ret>::push(L, (t->ref->*m)(converter<arg1>::get(L, -3), converter<arg2>::get(L, -2), converter<arg3>::get(L, -1)));
    }
    
  };
  
  template<typename T, typename arg1, typename arg2, typename arg3, typename arg4>
  struct method<T, void, arg1, arg2, arg3, arg4, empty> : public abstract_method {
    
    void (T::*m)(arg1, arg2, arg3, arg4);
    
    method(void (T::*m)(arg1, arg2, arg3, arg4)) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 5 && converter<arg1>::check(L, -4) && converter<arg2>::check(L, -3) && converter<arg3>::check(L, -2) && converter<arg4>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      (t->ref->*m)(converter<arg1>::get(L, -4), converter<arg2>::get(L, -3), converter<arg3>::get(L, -2), converter<arg4>::get(L, -1));
      return 0;
    }
    
  };
  
  template<typename T, typename ret, typename arg1, typename arg2, typename arg3, typename arg4>
  struct method<T, ret, arg1, arg2, arg3, arg4, empty> : public abstract_method {
    
    ret (T::*m)(arg1, arg2, arg3, arg4);
    
    method(ret (T::*m)(arg1, arg2, arg3, arg4)) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 5 && converter<arg1>::check(L, -4) && converter<arg2>::check(L, -3) && converter<arg3>::check(L, -2) && converter<arg4>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      return converter<ret>::push(L, (t->ref->*m)(converter<arg1>::get(L, -4), converter<arg2>::get(L, -3), converter<arg3>::get(L, -2), converter<arg4>::get(L, -1)));
    }
    
  };
  
  template<typename T, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5>
  struct method<T, void, arg1, arg2, arg3, arg4, arg5> : public abstract_method {
    
    void (T::*m)(arg1, arg2, arg3, arg4, arg5);
    
    method(void (T::*m)(arg1, arg2, arg3, arg4, arg5)) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 6 && converter<arg1>::check(L, -5) && converter<arg2>::check(L, -4) && converter<arg3>::check(L, -3) && converter<arg4>::check(L, -2)
           && converter<arg5>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      (t->ref->*m)(converter<arg1>::get(L, -5), converter<arg2>::get(L, -4), converter<arg3>::get(L, -3), converter<arg4>::get(L, -2), converter<arg5>::get(L, -1));
      return 0;
    }
    
  };
  
  template<typename T, typename ret = void, typename arg1 = empty, typename arg2 = empty, typename arg3 = empty, typename arg4 = empty>
  struct const_method : public abstract_method {
    
    ret (T::*m)(arg1, arg2, arg3, arg4) const;
    
    const_method(ret (T::*m)(arg1, arg2, arg3, arg4) const) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 5 && converter<arg1>::check(L, -4) && converter<arg2>::check(L, -3) && converter<arg3>::check(L, -2) && converter<arg4>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      return converter<ret>::push(L, (t->ref->*m)(converter<arg1>::get(L, -4), converter<arg2>::get(L, -3), converter<arg3>::get(L, -2), converter<arg4>::get(L, -1)));
    }
    
  };
  
  template<typename T>
  struct const_method<T, void, empty, empty, empty, empty> : public abstract_method {
    
    void (T::*m)() const;
    
    const_method(void (T::*m)() const) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 1;
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      (t->ref->*m)();
      return 0;
    }
    
  };
  
  template<typename T, typename ret>
  struct const_method<T, ret, empty, empty, empty, empty> : public abstract_method {
    
    ret (T::*m)() const;
    
    const_method(ret (T::*m)() const) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 1;
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      return converter<ret>::push(L, (t->ref->*m)());
    }
    
  };
  
  template<typename T, typename arg1>
  struct const_method<T, void, arg1, empty, empty, empty> : public abstract_method {
    
    void (T::*m)(arg1) const;
    
    const_method(void (T::*m)(arg1) const) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 2 && converter<arg1>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      (t->ref->*m)(converter<arg1>::get(L, -1));
      return 0;
    }
    
  };
  
  template<typename T, typename ret, typename arg1>
  struct const_method<T, ret, arg1, empty, empty, empty> : public abstract_method {
    
    ret (T::*m)(arg1) const;
    
    const_method(ret (T::*m)(arg1) const) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 2 && converter<arg1>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      return converter<ret>::push(L, (t->ref->*m)(converter<arg1>::get(L, -1)));
    }
    
  };
  
  template<typename T, typename arg1, typename arg2>
  struct const_method<T, void, arg1, arg2, empty, empty> : public abstract_method {
    
    void (T::*m)(arg1, arg2) const;
    
    const_method(void (T::*m)(arg1, arg2) const) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 3 && converter<arg1>::check(L, -2) && converter<arg2>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      (t->ref->*m)(converter<arg1>::get(L, -2), converter<arg2>::get(L, -1));
      return 0;
    }
    
  };

  template<typename T, typename ret, typename arg1, typename arg2>
  struct const_method<T, ret, arg1, arg2, empty, empty> : public abstract_method {
    
    ret (T::*m)(arg1, arg2) const;
    
    const_method(ret (T::*m)(arg1, arg2) const) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 3 && converter<arg1>::check(L, -2) && converter<arg2>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      return converter<ret>::push(L, (t->ref->*m)(converter<arg1>::get(L, -2), converter<arg2>::get(L, -1)));
    }
    
  };
  
  template<typename T, typename arg1, typename arg2, typename arg3>
  struct const_method<T, void, arg1, arg2, arg3, empty> : public abstract_method {
    
    void (T::*m)(arg1, arg2, arg3) const;
    
    const_method(void (T::*m)(arg1, arg2, arg3) const) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 4 && converter<arg1>::check(L, -3) && converter<arg2>::check(L, -2) && converter<arg3>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      (t->ref->*m)(converter<arg1>::get(L, -3), converter<arg2>::get(L, -2), converter<arg3>::get(L, -1));
      return 0;
    }
    
  };
  
  template<typename T, typename ret, typename arg1, typename arg2, typename arg3>
  struct const_method<T, ret, arg1, arg2, arg3, empty> : public abstract_method {
    
    ret (T::*m)(arg1, arg2, arg3) const;
    
    const_method(ret (T::*m)(arg1, arg2, arg3) const) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 4 && converter<arg1>::check(L, -3) && converter<arg2>::check(L, -2) && converter<arg3>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      return converter<ret>::push(L, (t->ref->*m)(converter<arg1>::get(L, -3), converter<arg2>::get(L, -2), converter<arg3>::get(L, -1)));
    }
    
  };
  
  template<typename T, typename arg1, typename arg2, typename arg3, typename arg4>
  struct const_method<T, void, arg1, arg2, arg3, arg4> : public abstract_method {
    
    void (T::*m)(arg1, arg2, arg3, arg4) const;
    
    const_method(void (T::*m)(arg1, arg2, arg3, arg4) const) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 5 && converter<arg1>::check(L, -4) && converter<arg2>::check(L, -3) && converter<arg3>::check(L, -2) && converter<arg4>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      (t->ref->*m)(converter<arg1>::get(L, -4), converter<arg2>::get(L, -3), converter<arg3>::get(L, -2), converter<arg4>::get(L, -1));
      return 0;
    }
    
  };
  
  template<typename T, typename ret = void, typename arg1 = empty, typename arg2 = empty, typename arg3 = empty, typename arg4 = empty>
  struct func_method : public abstract_method {
    
    ret (*m)(T*, arg1, arg2, arg3, arg4);
    
    func_method(ret (*m)(T*, arg1, arg2, arg3, arg4)) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 5 && converter<arg1>::check(L, -4) && converter<arg2>::check(L, -3) && converter<arg3>::check(L, -2) && converter<arg4>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      return converter<ret>::push(L, (*m)(t->ref, converter<arg1>::get(L, -4), converter<arg2>::get(L, -3), converter<arg3>::get(L, -2), converter<arg4>::get(L, -1)));
    }
    
  };
  
  template<typename T>
  struct func_method<T, void, empty, empty, empty, empty> : public abstract_method {
    
    void (*m)(T*);
    
    func_method(void (*m)(T*)) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 1;
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      (*m)(t->ref);
      return 0;
    }
    
  };
  
  template<typename T, typename ret>
  struct func_method<T, ret, empty, empty, empty, empty> : public abstract_method {
    
    ret (*m)(T*);
    
    func_method(ret (*m)(T*)) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 1;
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      return converter<ret>::push(L, (*m)(t->ref));
    }
    
  };
  
  template<typename T, typename arg1>
  struct func_method<T, void, arg1, empty, empty, empty> : public abstract_method {
    
    void (*m)(T*, arg1);
    
    func_method(void (*m)(T*, arg1)) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 2 && converter<arg1>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      (*m)(t->ref, converter<arg1>::get(L, -1));
      return 0;
    }
    
  };
  
  template<typename T, typename ret, typename arg1>
  struct func_method<T, ret, arg1, empty, empty, empty> : public abstract_method {
    
    ret (*m)(T*, arg1);
    
    func_method(ret (*m)(T*, arg1)) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 2 && converter<arg1>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      return converter<ret>::push(L, (*m)(t->ref, converter<arg1>::get(L, -1)));
    }
    
  };
  
  template<typename T, typename arg1, typename arg2>
  struct func_method<T, void, arg1, arg2, empty, empty> : public abstract_method {
    
    void (*m)(T*, arg1, arg2);
    
    func_method(void (*m)(T*, arg1, arg2)) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 3 && converter<arg1>::check(L, -2) && converter<arg2>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      (*m)(t->ref, converter<arg1>::get(L, -2), converter<arg2>::get(L, -1));
      return 0;
    }
    
  };
  
  template<typename T, typename ret, typename arg1, typename arg2>
  struct func_method<T, ret, arg1, arg2, empty, empty> : public abstract_method {
    
    ret (*m)(T*, arg1, arg2);
    
    func_method(ret (*m)(T*, arg1, arg2)) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 3 && converter<arg1>::check(L, -2) && converter<arg2>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      return converter<ret>::push(L, (*m)(t->ref, converter<arg1>::get(L, -2), converter<arg2>::get(L, -1)));
    }
    
  };
  
  template<typename T, typename arg1, typename arg2, typename arg3>
  struct func_method<T, void, arg1, arg2, arg3, empty> : public abstract_method {
    
    void (*m)(T*, arg1, arg2, arg3);
    
    func_method(void (*m)(T*, arg1, arg2, arg3)) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 4 && converter<arg1>::check(L, -3) && converter<arg2>::check(L, -2) && converter<arg3>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      (*m)(t->ref, converter<arg1>::get(L, -3), converter<arg2>::get(L, -2), converter<arg3>::get(L, -1));
      return 0;
    }
    
  };
  
  template<typename T, typename ret, typename arg1, typename arg2, typename arg3>
  struct func_method<T, ret, arg1, arg2, arg3, empty> : public abstract_method {
    
    ret (*m)(T*, arg1, arg2, arg3);
    
    func_method(ret (*m)(T*, arg1, arg2, arg3)) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 4 && converter<arg1>::check(L, -3) && converter<arg2>::check(L, -2) && converter<arg3>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      return converter<ret>::push(L, (*m)(t->ref, converter<arg1>::get(L, -3), converter<arg2>::get(L, -2), converter<arg3>::get(L, -1)));
    }
    
  };
  
  template<typename T, typename arg1, typename arg2, typename arg3, typename arg4>
  struct func_method<T, void, arg1, arg2, arg3, arg4> : public abstract_method {
    
    void (*m)(T*, arg1, arg2, arg3, arg4);
    
    func_method(void (*m)(T*, arg1, arg2, arg3, arg4)) : m(m) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 5 && converter<arg1>::check(L, -4) && converter<arg2>::check(L, -3) && converter<arg3>::check(L, -2) && converter<arg4>::check(L, -1);
    }
    
    int call(lua_State* L) {
      wrapper<T*>* t = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, 1));
      (*m)(t->ref, converter<arg1>::get(L, -4), converter<arg2>::get(L, -3), converter<arg3>::get(L, -2), converter<arg4>::get(L, -1));
      return 0;
    }
    
  };
  
}

#endif
