#ifndef SLUB_OPERATORS_H
#define SLUB_OPERATORS_H

#include "slub_lua.h"
#include "converter.h"

#include <sstream>

namespace slub {

  struct abstract_operator {

    virtual bool check(lua_State* L) = 0;
    virtual int op(lua_State* L) = 0;

  };

  template<typename T, typename F = empty>
  struct operator_ : public abstract_operator {

    bool check(lua_State* L) {
      return lua_gettop(L) == 2 && converter<T>::check(L, 1) && converter<F>::check(L, -1);
    }

  };

  template<typename T>
  struct operator_<T, empty> : public abstract_operator {
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 1 && converter<T>::check(L, 1);
    }
    
  };
  
  template<typename T, typename F>
  struct eq_operator : public operator_<T, F> {
    
    int op(lua_State* L) {
      return converter<bool>::push(L, *converter<T*>::get(L, 1) == converter<F>::get(L, -1));
    }
    
  };
  
  template<typename T, typename F>
  struct lt_operator : public operator_<T, F> {
    
    int op(lua_State* L) {
      return converter<bool>::push(L, *converter<T*>::get(L, 1) < converter<F>::get(L, -1));
    }
    
  };
  
  template<typename T, typename F>
  struct le_operator : public operator_<T, F> {
    
    int op(lua_State* L) {
      return converter<bool>::push(L, *converter<T*>::get(L, 1) <= converter<F>::get(L, -1));
    }
    
  };
  
  template<typename T>
  struct tostring_operator : public operator_<T, empty> {
    
    int op(lua_State* L) {
      std::stringstream s;
      s << *converter<T*>::get(L, 1);
      lua_pushstring(L, s.str().c_str());
      return 1;
    }
    
  };
  
  template<typename T, typename R, typename F>
  struct add_operator : public operator_<T, F> {

    int op(lua_State* L) {
      return converter<R>::push(L, *converter<T*>::get(L, 1) + converter<F>::get(L, -1));
    }

  };
  
  template<typename T, typename R, typename F>
  struct sub_operator : public operator_<T, F> {
    
    int op(lua_State* L) {
      return converter<R>::push(L, *converter<T*>::get(L, 1) - converter<F>::get(L, -1));
    }
           
  };
           
  template<typename T, typename R, typename F>
  struct mul_operator : public operator_<T, F> {
    
    int op(lua_State* L) {
      return converter<R>::push(L, *converter<T*>::get(L, 1) * converter<F>::get(L, -1));
    }
           
  };
           
  template<typename T, typename R, typename F>
  struct div_operator : public operator_<T, F> {
    
    int op(lua_State* L) {
      return converter<R>::push(L, *converter<T*>::get(L, 1) / converter<F>::get(L, -1));
    }
           
  };
           
  template<typename T, typename R, typename F>
  struct mod_operator : public operator_<T, F> {
    
    int op(lua_State* L) {
      return converter<R>::push(L, *converter<T*>::get(L, 1) % converter<F>::get(L, -1));
    }
           
  };
           
  template<typename T, typename R, typename F>
  struct pow_operator : public operator_<T, F> {
    
    int op(lua_State* L) {
      return converter<R>::push(L, *converter<T*>::get(L, 1) ^ converter<F>::get(L, -1));
    }
           
  };
           
  template<typename T, typename R>
  struct unm_operator : public operator_<T, empty> {
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 2 && converter<T>::check(L, 1);
    }

    int op(lua_State* L) {
      return converter<R>::push(L, -(*converter<T*>::get(L, 1)));
    }
    
  };
  
}

#endif
