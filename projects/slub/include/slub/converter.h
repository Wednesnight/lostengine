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

#ifndef SLUB_CONVERTER_H
#define SLUB_CONVERTER_H

#include "config.h"
#include "registry.h"
#include "wrapper.h"

#include <stdexcept>
#include <iostream>
#include <typeinfo>

namespace boost {
  template<class T> class shared_ptr;
}

namespace std {
  namespace tr1 {
    template<class T> class shared_ptr;
  }
}

namespace slub {

  template<typename T>
  struct converter {

    static bool check(lua_State* L, int index) {
      return converter<T*>::check(L, index);
    }

    static void* checkudata(lua_State* L, int index) {
      return converter<T*>::checkudata(L, index);
    }

    static T& get(lua_State* L, int index) {
      if (registry::isRegisteredType<T>()) {
//        std::cout << "get, registered" << std::endl;
        wrapper<T*>* w = static_cast<wrapper<T*>*>(checkudata(L, index));
        return *w->ref;
      }
      throw std::runtime_error("trying to use unregistered type");
    }

    static int push(lua_State* L, const T& value) {
      if (registry::isRegisteredType<T>()) {
//        std::cout << "push, registered" << std::endl;
        wrapper<T*>* w = wrapper<T*>::create(L, typeid(value));
        w->ref = new T(value);
        w->gc = true;
        luaL_getmetatable(L, registry::get(typeid(value))->getTypeName().c_str());
        lua_setmetatable(L, -2);
        return 1;
      }
      throw std::runtime_error("trying to use unregistered type");
    }

  };

  template<typename T>
  struct converter<T*> {

    static bool checkBases(registry* reg) {
      bool result = false;
      const list<registry*> base = reg->baseList();
      for (list<registry*>::const_iterator idx = base.begin(); !result && idx != base.end(); ++idx) {
        result = (*idx)->getType() == typeid(T);
        if (!result && (*idx)->hasBase()) {
          result = checkBases(*idx);
        }
      }
      return result;
    }

    static bool check(lua_State* L, int index) {
      bool result = false;
      wrapper_base* w = (wrapper_base*) lua_touserdata(L, index);
      if (w != NULL) {  // value is a userdata?
        result = *w->type == typeid(T);
        if (!result) {
          registry* reg = registry::get(*w->type);
          if (reg != NULL && reg->hasBase()) {
            result = checkBases(reg);
          }
        }
      }
      return result;
    }
    
    static void* checkudata(lua_State* L, int index) {
      if (!check(L, index)) {
        luaL_typerror(L, index, registry::get(typeid(T))->getTypeName().c_str());
      }
      return lua_touserdata(L, index);
    }
    
    static T* get(lua_State* L, int index) {
      if (registry::isRegisteredType<T>()) {
          //        std::cout << "get, registered" << std::endl;
        wrapper<T*>* w = static_cast<wrapper<T*>*>(converter<T*>::checkudata(L, index));
        return w->ref;
      }
      throw std::runtime_error("trying to use unregistered type");
    }
    
    static int push(lua_State* L, T* value) {
      return push(L, value, false);
    }
    
    static int push(lua_State* L, T* value, bool gc) {
      if (registry::isRegisteredType<T>()) {
          //        std::cout << "push, registered" << std::endl;
        wrapper<T*>* w = wrapper<T*>::create(L, typeid(*value));
        w->ref = value;
        w->gc = gc;
        luaL_getmetatable(L, registry::get(typeid(*value))->getTypeName().c_str());
        lua_setmetatable(L, -2);
        return 1;
      }
      throw std::runtime_error("trying to use unregistered type");
    }
    
  };
  
  template<typename T>
  struct converter<boost::shared_ptr<T> > {
    
    static bool check(lua_State* L, int index) {
      return converter<T>::check(L, index);
    }
    
    static boost::shared_ptr<T>& get(lua_State* L, int index) {
      if (registry::isRegisteredType<T>()) {
//        std::cout << "get, registered" << std::endl;
        wrapper<T*, boost::shared_ptr<T>*>* w = static_cast<wrapper<T*, boost::shared_ptr<T>*>*>(converter<T>::checkudata(L, index));
        return *(w->holder);
      }
      throw std::runtime_error("trying to use unregistered type");
    }
    
    static int push(lua_State* L, const boost::shared_ptr<T>& value) {
      if (registry::isRegisteredType<T>()) {
//        std::cout << "push, registered" << std::endl;
        const std::type_info* type = &typeid(*(value.get()));
        registry* reg = registry::get(*type);
        if (reg == NULL) {
          type = &typeid(T);
          reg = registry::get(*type);
        }
        if (reg != NULL) {
          wrapper<T*, boost::shared_ptr<T>*>* w = wrapper<T*, boost::shared_ptr<T>*>::create(L, *type);
          w->holder = new boost::shared_ptr<T>(value);
          w->ref = value.get();
          w->gc = true;
          luaL_getmetatable(L, reg->getTypeName().c_str());
          lua_setmetatable(L, -2);
          return 1;
        }
      }
      throw std::runtime_error("trying to use unregistered type");
    }

  };

  template<typename T>
  struct converter<boost::shared_ptr<T>&> : converter<boost::shared_ptr<T> > {};
  
  template<typename T>
  struct converter<const boost::shared_ptr<T>&> : converter<boost::shared_ptr<T> > {};
  
  template<typename T>
  struct converter<std::tr1::shared_ptr<T> > {
    
    static bool check(lua_State* L, int index) {
      return converter<T>::check(L, index);
    }
    
    static std::tr1::shared_ptr<T>& get(lua_State* L, int index) {
      if (registry::isRegisteredType<T>()) {
//        std::cout << "get, registered" << std::endl;
        wrapper<T*, std::tr1::shared_ptr<T>*>* w = static_cast<wrapper<T*, std::tr1::shared_ptr<T>*>*>(converter<T>::checkudata(L, index));
        return *(w->holder);
      }
      throw std::runtime_error("trying to use unregistered type");
    }
    
    static int push(lua_State* L, const std::tr1::shared_ptr<T>& value) {
      if (registry::isRegisteredType<T>()) {
//        std::cout << "push, registered" << std::endl;
        const std::type_info* type = &typeid(*(value.get()));
        registry* reg = registry::get(*type);
        if (reg == NULL) {
          type = &typeid(T);
          reg = registry::get(*type);
        }
        if (reg != NULL) {
          wrapper<T*, std::tr1::shared_ptr<T>*>* w = wrapper<T*, std::tr1::shared_ptr<T>*>::create(L, *type);
          w->holder = new std::tr1::shared_ptr<T>(value);
          w->ref = value.get();
          w->gc = true;
          luaL_getmetatable(L, reg->getTypeName().c_str());
          lua_setmetatable(L, -2);
          return 1;
        }
      }
      throw std::runtime_error("trying to use unregistered type");
    }
    
  };
  
  template<typename T>
  struct converter<std::tr1::shared_ptr<T>&> : converter<std::tr1::shared_ptr<T> > {};

  template<typename T>
  struct converter<const std::tr1::shared_ptr<T>&> : converter<std::tr1::shared_ptr<T> > {};
  
  template<typename T>
  struct converter<const T*> {
    
    static bool check(lua_State* L, int index) {
      return converter<T>::check(L, index);
    }
    
    static const T* get(lua_State* L, int index) {
      if (registry::isRegisteredType<T>()) {
//        std::cout << "get, registered" << std::endl;
        wrapper<const T*>* w = static_cast<wrapper<const T*>*>(converter<T>::checkudata(L, index));
        return w->ref;
      }
      throw std::runtime_error("trying to use unregistered type");
    }
    
    static int push(lua_State* L, const T* value) {
      return push(L, value, false);
    }
    
    static int push(lua_State* L, const T* value, bool gc) {
      if (registry::isRegisteredType<T>()) {
//        std::cout << "push, registered" << std::endl;
        wrapper<const T*>* w = wrapper<const T*>::create(L, typeid(*value));
        w->ref = value;
        w->gc = gc;
        luaL_getmetatable(L, registry::get(typeid(*value))->getTypeName().c_str());
        lua_setmetatable(L, -2);
        return 1;
      }
      throw std::runtime_error("trying to use unregistered type");
    }
    
  };
  
  template<typename T>
  struct converter<T&> {
    
    static bool check(lua_State* L, int index) {
      return converter<T>::check(L, index);
    }
    
    static T& get(lua_State* L, int index) {
      if (registry::isRegisteredType<T>()) {
//        std::cout << "get, registered" << std::endl;
        wrapper<T*>* w = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, index));
        return *w->ref;
      }
      throw std::runtime_error("trying to use unregistered type");
    }
    
    static int push(lua_State* L, T& value) {
      return push(L, value, false);
    }
    
    static int push(lua_State* L, T& value, bool gc) {
      if (registry::isRegisteredType<T>()) {
//        std::cout << "push, registered" << std::endl;
        wrapper<T*>* w = wrapper<T*>::create(L, typeid(value));
        w->ref = &value;
        w->gc = gc;
        luaL_getmetatable(L, registry::get(typeid(value))->getTypeName().c_str());
        lua_setmetatable(L, -2);
        return 1;
      }
      throw std::runtime_error("trying to use unregistered type");
    }
    
  };
  
  template<typename T>
  struct converter<const T&> {
    
    static bool check(lua_State* L, int index) {
      return converter<T>::check(L, index);
    }
    
    static const T& get(lua_State* L, int index) {
      if (registry::isRegisteredType<T>()) {
//        std::cout << "get, registered" << std::endl;
        wrapper<const T*>* w = static_cast<wrapper<const T*>*>(converter<T>::checkudata(L, index));
        return *w->ref;
      }
      throw std::runtime_error("trying to use unregistered type");
    }
    
    static int push(lua_State* L, const T& value) {
      return push(L, value, false);
    }
    
    static int push(lua_State* L, const T& value, bool gc) {
      if (registry::isRegisteredType<T>()) {
//        std::cout << "push, registered" << std::endl;
        wrapper<const T*>* w = wrapper<const T*>::create(L, typeid(value));
        w->ref = &value;
        w->gc = gc;
        luaL_getmetatable(L, registry::get(typeid(value))->getTypeName().c_str());
        lua_setmetatable(L, -2);
        return 1;
      }
      return 0;
    }
    
  };
  
  template<>
  struct converter<bool> {
    
    static bool check(lua_State* L, int index) {
      return lua_isboolean(L, index);
    }
    
    static bool get(lua_State* L, int index) {
      luaL_checktype(L, index, LUA_TBOOLEAN);
      return lua_toboolean(L, index);
    }
    
    static int push(lua_State* L, bool value) {
      lua_pushboolean(L, value);
      return 1;
    }
    
  };
  
  template<>
  struct converter<int> {

    static bool check(lua_State* L, int index) {
      return lua_isnumber(L, index);
    }
    
    static int get(lua_State* L, int index) {
      return luaL_checkinteger(L, index);
    }

    static int push(lua_State* L, int value) {
      lua_pushinteger(L, value);
      return 1;
    }

  };

  template<>
  struct converter<const int&> : converter<int> {};
  
  template<>
  struct converter<unsigned int> {
    
    static bool check(lua_State* L, int index) {
      return lua_isnumber(L, index);
    }
    
    static unsigned int get(lua_State* L, int index) {
      return luaL_checkinteger(L, index);
    }
    
    static int push(lua_State* L, unsigned int value) {
      lua_pushinteger(L, value);
      return 1;
    }
    
  };
  
  template<>
  struct converter<const unsigned int&> : converter<unsigned int> {};
  
  template<>
  struct converter<unsigned short> {
    
    static bool check(lua_State* L, int index) {
      return lua_isnumber(L, index);
    }
    
    static unsigned short get(lua_State* L, int index) {
      return luaL_checkinteger(L, index);
    }
    
    static int push(lua_State* L, unsigned short value) {
      lua_pushinteger(L, value);
      return 1;
    }
    
  };
  
  template<>
  struct converter<const unsigned short&> : converter<unsigned short> {};
  
  template<>
  struct converter<unsigned char> {
    
    static bool check(lua_State* L, int index) {
      return lua_isnumber(L, index);
    }
    
    static unsigned char get(lua_State* L, int index) {
      return luaL_checkinteger(L, index);
    }
    
    static int push(lua_State* L, unsigned char value) {
      lua_pushinteger(L, value);
      return 1;
    }
    
  };
  
  template<>
  struct converter<const unsigned char&> : converter<unsigned char> {};
  
  template<>
  struct converter<double> {
    
    static bool check(lua_State* L, int index) {
      return lua_isnumber(L, index);
    }
    
    static double get(lua_State* L, int index) {
      return luaL_checknumber(L, index);
    }
    
    static int push(lua_State* L, double value) {
      lua_pushnumber(L, value);
      return 1;
    }
    
  };

  template<>
  struct converter<float> : converter<double> {};

  template<>
  struct converter<char*> {
    
    static bool check(lua_State* L, int index) {
      return lua_isstring(L, index);
    }
    
    static const char* get(lua_State* L, int index) {
      return luaL_checkstring(L, index);
    }
    
    static int push(lua_State* L, char* value) {
      lua_pushstring(L, value);
      return 1;
    }
    
  };
  
  template<>
  struct converter<const char*> {
    
    static bool check(lua_State* L, int index) {
      return lua_isstring(L, index);
    }
    
    static const char* get(lua_State* L, int index) {
      return luaL_checkstring(L, index);
    }
    
    static int push(lua_State* L, const char* value) {
      lua_pushstring(L, value);
      return 1;
    }
    
  };
  
  template<>
  struct converter<string> {
    
    static bool check(lua_State* L, int index) {
      return lua_isstring(L, index);
    }
    
    static string get(lua_State* L, int index) {
      return luaL_checkstring(L, index);
    }
    
    static int push(lua_State* L, const string& value) {
      lua_pushstring(L, value.c_str());
      return 1;
    }
    
  };
  
  template<>
  struct converter<string&> : converter<string> {};
  
  template<>
  struct converter<const string&> : converter<string> {};
  
  template<>
  struct converter<string*> {
    
    static bool check(lua_State* L, int index) {
      return lua_isstring(L, index);
    }
    
    static string* get(lua_State* L, int index) {
      throw std::runtime_error("Cannot get a string* value");
    }
    
    static int push(lua_State* L, const string* value) {
      lua_pushstring(L, value->c_str());
      return 1;
    }
    
  };

  template<>
  struct converter<const string*> : converter<string*> {};

  template<>
  struct converter<void*> {
    
    static bool check(lua_State* L, int index) {
      return lua_isuserdata(L, index);
    }
    
    static void* get(lua_State* L, int index) {
      luaL_checktype(L, index, LUA_TLIGHTUSERDATA);
      return lua_touserdata(L, index);
    }
    
    static int push(lua_State* L, void* value) {
      lua_pushlightuserdata(L, value);
      return 1;
    }
    
  };
  
}

#endif
