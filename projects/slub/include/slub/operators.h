/*
Copyright (c) 2011 Timo Boll, Tony Kostanjsek

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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
