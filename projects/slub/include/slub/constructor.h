#ifndef SLUB_CONSTRUCTOR_H
#define SLUB_CONSTRUCTOR_H

#include <slub/slub_lua.h>
#include <slub/converter.h>

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
