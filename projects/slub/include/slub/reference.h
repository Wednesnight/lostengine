#ifndef SLUB_REFERENCE_H
#define SLUB_REFERENCE_H

#include "converter.h"

#include <list>
#include <string>
#include <stdexcept>

namespace slub {

  template<typename T>
  struct converter;

  struct reference {

    template<typename T>
    friend struct converter;

    reference() : state(NULL), index(LUA_REFNIL) {
    }
    
    reference(lua_State* state) : state(state), index(luaL_ref(state, LUA_REGISTRYINDEX)) {
    }
    
    reference(lua_State* state, int index) : state(state) {
      lua_pushvalue(state, index);
      this->index = luaL_ref(state, LUA_REGISTRYINDEX);
    }
    
    reference(const reference& r) {
      *this = r;
    }
    
    ~reference() {
      if (state != NULL) {
        luaL_unref(state, LUA_REGISTRYINDEX, index);
      }
    }
    
    void operator=(const reference& r) {
      this->state = r.state;
      r.push();
      this->index = luaL_ref(state, LUA_REGISTRYINDEX);
    }
    
    int type() const {
      int result = lua_type(state, push());
      pop();
      return result;
    }
    
    std::string typeName() const {
      return lua_typename(state, type());
    }
    
    reference operator[](const std::string& name) const {
      lua_getfield(state, push(), name.c_str());
      reference result(state);
      pop();
      
      result.name = name;
      if (this->path.size() > 0) {
        result.path.insert(result.path.begin(), this->path.begin(), this->path.end());
      }
      result.path.push_back(this->name);
      
      return result;
    }
    
    template<typename T>
    void operator=(T value) {
      if (name.size() == 0) {
        throw std::runtime_error("trying to modify stack reference");
      }
      
      int index = LUA_GLOBALSINDEX;
      for (std::list<std::string>::iterator iter = path.begin(); iter != path.end(); ++iter) {
        lua_getfield(state, index, iter->c_str());
        index = lua_gettop(state);
      }
      converter<T>::push(state, value);
      lua_setfield(state, index, name.c_str());
      lua_pop(state, path.size());
    }
    
    template<typename T>
    T cast() const {
      T result = converter<T>::get(state, push());
      pop();
      return result;
    }
    
    template<typename R>
    R operator()() const {
      push();
      lua_call(state, 0, 1);
      R r = converter<R>::get(state, -1);
      lua_pop(state, 1);
      return r;
    }
    
    void operator()() const {
      push();
      lua_call(state, 0, 0);
    }
    
    template<typename R, typename arg1>
    R operator()(arg1 a1) const {
      push();
      converter<arg1>::push(state, a1);
      lua_call(state, 1, 1);
      R r = converter<R>::get(state, -1);
      lua_pop(state, 1);
      return r;
    }
    
    template<typename arg1>
    void operator()(arg1 a1) const {
      push();
      converter<arg1>::push(state, a1);
      lua_call(state, 1, 0);
    }
    
    template<typename R, typename arg1, typename arg2>
    R operator()(arg1 a1, arg2 a2) const {
      push();
      converter<arg1>::push(state, a1);
      converter<arg2>::push(state, a2);
      lua_call(state, 2, 1);
      R r = converter<R>::get(state, -1);
      lua_pop(state, 1);
      return r;
    }
    
    template<typename arg1, typename arg2>
    void operator()(arg1 a1, arg2 a2) const {
      push();
      converter<arg1>::push(state, a1);
      converter<arg2>::push(state, a2);
      lua_call(state, 2, 0);
    }
    
  protected:
    
    friend struct globals;
    
    lua_State* state;
    int index;
    
    std::string name;
    std::list<std::string> path;
    
    int push() const {
      lua_rawgeti(state, LUA_REGISTRYINDEX, index);
      return lua_gettop(state);
    }
    
    void pop() const {
      lua_pop(state, 1);
    }
    
  };
  
}

#endif
