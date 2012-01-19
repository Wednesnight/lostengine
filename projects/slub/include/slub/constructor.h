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

#ifndef SLUB_CONSTRUCTOR_H
#define SLUB_CONSTRUCTOR_H

#include "slub_lua.h"
#include "converter.h"

namespace slub {

  struct abstract_constructor {

    virtual bool check(lua_State* L) = 0;
    virtual void* _newInstance(lua_State* L) = 0;

    template<typename T>
    T* newInstance(lua_State* L) {
      return (T*) _newInstance(L);
    }

  };

  template<typename T, typename arg1 = empty, typename arg2 = empty, typename arg3 = empty, typename arg4 = empty>
  struct constructor : public abstract_constructor {

    bool check(lua_State* L) {
      return lua_gettop(L) == 5 && converter<arg1>::check(L, -4) && converter<arg2>::check(L, -3) && converter<arg3>::check(L, -2) && converter<arg4>::check(L, -1);
    }
    
    void* _newInstance(lua_State* L) {
      return new T(converter<arg1>::get(L, -4), converter<arg2>::get(L, -3), converter<arg3>::get(L, -2), converter<arg4>::get(L, -1));
    }
    
  };
  
  template<typename T>
  struct constructor<T, empty, empty, empty, empty> : public abstract_constructor {
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 1;
    }
    
    void* _newInstance(lua_State* L) {
      return new T();
    }
    
  };
  
  template<typename T, typename arg1>
  struct constructor<T, arg1, empty, empty, empty> : public abstract_constructor {
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 2 && converter<arg1>::check(L, -1);
    }
    
    void* _newInstance(lua_State* L) {
      return new T(converter<arg1>::get(L, -1));
    }
    
  };
  
  template<typename T, typename arg1, typename arg2>
  struct constructor<T, arg1, arg2, empty, empty> : public abstract_constructor {
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 3 && converter<arg1>::check(L, -2) && converter<arg2>::check(L, -1);
    }
    
    void* _newInstance(lua_State* L) {
      return new T(converter<arg1>::get(L, -2), converter<arg2>::get(L, -1));
    }
    
  };
  
  template<typename T, typename arg1, typename arg2, typename arg3>
  struct constructor<T, arg1, arg2, arg3, empty> : public abstract_constructor {
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 4 && converter<arg1>::check(L, -3) && converter<arg2>::check(L, -2) && converter<arg3>::check(L, -1);
    }
    
    void* _newInstance(lua_State* L) {
      return new T(converter<arg1>::get(L, -3), converter<arg2>::get(L, -2), converter<arg3>::get(L, -1));
    }
    
  };
  
}

#endif
