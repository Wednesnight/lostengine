/*
The MIT License (MIT)
Copyright (c) 2011 Timo Boll

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

#ifndef SLUB_PACKAGE_H
#define SLUB_PACKAGE_H

#include "config.h"
#include "clazz.h"

namespace slub {

  struct package_ {

    lua_State* state;
    int table;
    string name;

    package_(lua_State* L, const string& name, const string& prefix = "", int target = -1) : state(L) {
      lua_getfield(L, target != -1 ? target : LUA_GLOBALSINDEX, name.c_str());
      if (lua_type(L, lua_gettop(L)) == LUA_TNIL) {
//        std::cout << "creating package: " << name << std::endl;
        lua_pop(L, 1);
        lua_newtable(L);
        lua_pushvalue(L, -1);
        lua_setfield(L, target != -1 ? target : LUA_GLOBALSINDEX, name.c_str());
      }
      else {
//        std::cout << "using package: " << name << std::endl;
      }
      table = luaL_ref(L, LUA_REGISTRYINDEX);
      this->name = prefix.size() > 0 ? prefix +"."+ name : name;
    }

    package_ package(const string& name) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      package_ result(state, name, this->name, lua_gettop(state));
      lua_pop(state, 1);
      return result;
    }

    template<typename T>
    slub::clazz<T> clazz(const string& name) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::clazz<T> result(state, name, this->name, lua_gettop(state));
      lua_pop(state, 1);
      return result;
    }

    template<typename T, typename D>
    slub::clazz<T, D> clazz(const string& name) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::clazz<T, D> result(state, name, this->name, lua_gettop(state));
      lua_pop(state, 1);
      return result;
    }
    
    package_& function(const string& name, void (*f)()) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 1);
      return *this;
    }
    
    template<typename arg1>
    package_& function(const string& name, void (*f)(arg1)) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<arg1>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 1);
      return *this;
    }
    
    template<typename arg1, typename arg2>
    package_& function(const string& name, void (*f)(arg1, arg2)) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<arg1, arg2>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 1);
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3>
    package_& function(const string& name, void (*f)(arg1, arg2, arg3)) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<arg1, arg2, arg3>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 1);
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3, typename arg4>
    package_& function(const string& name, void (*f)(arg1, arg2, arg3, arg4)) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<arg1, arg2, arg3, arg4>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 1);
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3, typename arg4, typename arg5>
    package_& function(const string& name, void (*f)(arg1, arg2, arg3, arg4, arg5)) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<arg1, arg2, arg3, arg4, arg5>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 1);
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6>
    package_& function(const string& name, void (*f)(arg1, arg2, arg3, arg4, arg5, arg6)) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<arg1, arg2, arg3, arg4, arg5, arg6>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 1);
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6, typename arg7>
    package_& function(const string& name, void (*f)(arg1, arg2, arg3, arg4, arg5, arg6, arg7)) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<arg1, arg2, arg3, arg4, arg5, arg6, arg7>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 1);
      return *this;
    }
    
    template<typename R>
    package_& function(const string& name, R (*f)()) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<R>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 1);
      return *this;
    }
    
    template<typename R, typename arg1>
    package_& function(const string& name, R (*f)(arg1)) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<R, arg1>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 1);
      return *this;
    }
    
    template<typename R, typename arg1, typename arg2>
    package_& function(const string& name, R (*f)(arg1, arg2)) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<R, arg1, arg2>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 1);
      return *this;
    }
    
    template<typename R, typename arg1, typename arg2, typename arg3>
    package_& function(const string& name, R (*f)(arg1, arg2, arg3)) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<R, arg1, arg2, arg3>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 1);
      return *this;
    }
    
    template<typename R, typename arg1, typename arg2, typename arg3, typename arg4>
    package_& function(const string& name, R (*f)(arg1, arg2, arg3, arg4)) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<R, arg1, arg2, arg3, arg4>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 1);
      return *this;
    }
    
    template<typename R, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5>
    package_& function(const string& name, R (*f)(arg1, arg2, arg3, arg4, arg5)) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<R, arg1, arg2, arg3, arg4, arg5>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 1);
      return *this;
    }
    
    template<typename R, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6>
    package_& function(const string& name, R (*f)(arg1, arg2, arg3, arg4, arg5, arg6)) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<R, arg1, arg2, arg3, arg4, arg5, arg6>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 1);
      return *this;
    }
    
    template<typename R, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6, typename arg7>
    package_& function(const string& name, R (*f)(arg1, arg2, arg3, arg4, arg5, arg6, arg7)) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      slub::function<R, arg1, arg2, arg3, arg4, arg5, arg6, arg7>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 1);
      return *this;
    }
    
    template<typename T>
    package_& enumerated(const string& constantName, const T& value) {
      constant<int>(constantName, value);
      return *this;
    }
    
    template<typename T>
    package_& constant(const string& constantName, const T& value) {
      lua_rawgeti(state, LUA_REGISTRYINDEX, table);
      converter<T>::push(state, value);
      lua_setfield(state, -2, constantName.c_str());
      lua_pop(state, 1);
      return *this;
    }

  };

  typedef package_ package;

}

#endif
