#ifndef SLUB_REFERENCE_H
#define SLUB_REFERENCE_H

#include <string>
#include <stdexcept>
#include <slub/converter.h>

namespace slub {

  template<typename T>
  struct converter;

  struct reference {

  public:

    static reference newtable(lua_State* state) {
      lua_newtable(state);
      reference result(state, -1);
      lua_pop(state, 1);
      return result;
    }

  private:

    enum Mode {
      NONE,
      GLOBAL,
      NESTED,
      REFERENCE
    };

    Mode mode;
    lua_State* state;
    std::string field;
    int parent;
    int ref;

    reference(lua_State* state, const std::string& field, int index) : mode(NESTED), state(state), ref(LUA_NOREF), field(field) {
      lua_pushvalue(state, index);
      parent = luaL_ref(state, LUA_REGISTRYINDEX);
    }
    
    reference(lua_State* state, const std::string& field) : mode(GLOBAL), state(state), parent(LUA_NOREF), ref(LUA_NOREF), field(field) {
    }

  public:

    reference(const reference& r) {
      mode = r.mode;
      state = r.state;
      field = r.field;
      if (mode == NESTED) {
        lua_rawgeti(state, LUA_REGISTRYINDEX, r.parent);
        parent = luaL_ref(state, LUA_REGISTRYINDEX);
        ref = LUA_NOREF;
      }
      else if (mode == REFERENCE) {
        lua_rawgeti(state, LUA_REGISTRYINDEX, r.ref);
        ref = luaL_ref(state, LUA_REGISTRYINDEX);
        parent = LUA_NOREF;
      }
    }

    reference() : mode(NONE), state(NULL), ref(LUA_NOREF), parent(LUA_NOREF) {
    }
    
    reference(lua_State* state) : mode(GLOBAL), state(state), ref(LUA_NOREF), parent(LUA_NOREF) {
    }

    reference(lua_State* state, int index) : mode(REFERENCE), state(state), parent(LUA_NOREF) {
      lua_pushvalue(state, index);
      ref = luaL_ref(state, LUA_REGISTRYINDEX);
    }
    
    ~reference() {
      if (ref != LUA_NOREF && ref != LUA_REFNIL) {
        luaL_unref(state, LUA_REGISTRYINDEX, ref);
      }
      if (parent != LUA_NOREF && parent != LUA_REFNIL) {
        luaL_unref(state, LUA_REGISTRYINDEX, parent);
      }
    }

    reference operator[](const std::string& field) {
      if (mode == NONE) {
        throw std::runtime_error("trying to use uninitialized reference");
      }

      if (mode == GLOBAL) {
        if (this->field.size() > 0) {
          lua_getfield(state, LUA_GLOBALSINDEX, this->field.c_str());
          reference r(state, field, -1);
          lua_pop(state, 1);
          return r;
        }
        else {
          return reference(state, field);
        }
      }
      else if (mode == REFERENCE) {
        lua_rawgeti(state, LUA_REGISTRYINDEX, ref);
        reference r(state, field, -1);
        lua_pop(state, 1);
        return r;
      }
      else {
        lua_rawgeti(state, LUA_REGISTRYINDEX, parent);
        reference r(state, field, -1);
        lua_pop(state, 1);
        return r;
      }
    }

    int type() const {
      if (mode == GLOBAL) {
        if (field.size() > 0) {
          lua_getfield(state, LUA_GLOBALSINDEX, field.c_str());
          int result = lua_type(state, -1);
          lua_pop(state, 1);
          return result;
        }
        else {
          return lua_type(state, LUA_GLOBALSINDEX);
        }
      }
      else if (mode == REFERENCE) {
        lua_rawgeti(state, LUA_REGISTRYINDEX, ref);
        int result = lua_type(state, -1);
        lua_pop(state, 1);
        return result;
      }
      else if (mode == NESTED) {
        lua_rawgeti(state, LUA_REGISTRYINDEX, parent);
        lua_getfield(state, -1, field.c_str());
        int result = lua_type(state, -1);
        lua_pop(state, 2);
        return result;
      }
      else {
        return LUA_TNIL;
      }
    }

    template<typename T>
    void operator=(T t) {
      if (mode == NONE) {
        throw std::runtime_error("trying to use uninitialized reference");
      }

      if (mode == GLOBAL) {
        if (field.size() == 0) {
          throw std::runtime_error("trying to modify globals table");
        }

        converter<T>::push(state, t);
        lua_setfield(state, LUA_GLOBALSINDEX, field.c_str());
      }
      else if (mode == NESTED) {
        lua_rawgeti(state, LUA_REGISTRYINDEX, parent);
        converter<T>::push(state, t);
        lua_setfield(state, -2, field.c_str());
        lua_pop(state, 1);
      }
      else {
        throw std::runtime_error("trying to modify reference value");
      }
    }

    template<typename T>
    T get() const {
      if (mode == NONE) {
        throw std::runtime_error("trying to use uninitialized reference");
      }

      if (mode == REFERENCE) {
        lua_rawgeti(state, LUA_REGISTRYINDEX, ref);
        T result = converter<T>::get(state, -1);
        lua_pop(state, 1);
        return result;
      }
      else if (mode == GLOBAL) {
        if (field.size() == 0) {
          throw std::runtime_error("trying to cast globals table");
        }
        
        lua_getfield(state, LUA_GLOBALSINDEX, field.c_str());
        T result = converter<T>::get(state, -1);
        lua_pop(state, 1);
        return result;
      }
      else {
        lua_rawgeti(state, LUA_REGISTRYINDEX, parent);
        lua_getfield(state, -1, field.c_str());
        T result = converter<T>::get(state, -1);
        lua_pop(state, 2);
        return result;
      }
    }
    
    int push() const {
      if (mode == GLOBAL) {
        if (field.size() > 0) {
          lua_getfield(state, LUA_GLOBALSINDEX, field.c_str());
        }
        else {
          lua_pushvalue(state, LUA_GLOBALSINDEX);
        }
      }
      else if (mode == NESTED) {
        lua_rawgeti(state, LUA_REGISTRYINDEX, parent);
        lua_getfield(state, -1, field.c_str());
        lua_remove(state, -2);
      }
      else if (mode == REFERENCE) {
        lua_rawgeti(state, LUA_REGISTRYINDEX, ref);
      }
      else {
        lua_pushnil(state);
      }
      return 1;
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
    
  };

}

#endif
