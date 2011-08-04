#ifndef SLUB_CONVERTER_H
#define SLUB_CONVERTER_H

#include <slub/reference.h>
#include <slub/registry.h>
#include <slub/wrapper.h>
#include <stdexcept>
#include <iostream>
#include <list>

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
      bool result = false;
      void *p = lua_touserdata(L, index);
      if (p != NULL) {  /* value is a userdata? */
        if (lua_getmetatable(L, index)) {  /* does it have a metatable? */
          lua_getfield(L, LUA_REGISTRYINDEX, registry::getTypeName<T*>().c_str());  /* get correct metatable */
          if (lua_rawequal(L, -1, -2)) {  /* does it have the correct mt? */
            result = true;
          }
          else {
            // downcast
            if (registry::get<T*>()->isBase()) {
              const std::list<registry*> derived = registry::get<T*>()->derivedList();
              for (std::list<registry*>::const_iterator idx = derived.begin(); !result && idx != derived.end(); ++idx) {
                lua_pop(L, 1);
                lua_getfield(L, LUA_REGISTRYINDEX, (*idx)->getTypeName().c_str());
                if (lua_rawequal(L, -1, -2)) {  /* does it have the correct mt? */
                  result = true;
                }
              }
            }
            // upcast
            if (!result && registry::get<T*>()->hasBase()) {
              const std::list<registry*> base = registry::get<T*>()->baseList();
              for (std::list<registry*>::const_iterator idx = base.begin(); !result && idx != base.end(); ++idx) {
                lua_pop(L, 1);
                lua_getfield(L, LUA_REGISTRYINDEX, (*idx)->getTypeName().c_str());
                if (lua_rawequal(L, -1, -2)) {  /* does it have the correct mt? */
                  result = true;
                }
              }
            }
          }
          lua_pop(L, 2);  /* remove both metatables */
        }
      }
      return result;
    }

    static void* checkudata(lua_State* L, int index) {
      if (!check(L, index)) {
        luaL_typerror(L, index, registry::getTypeName<T*>().c_str());
      }
      return lua_touserdata(L, index);
    }

    static T get(lua_State* L, int index) {
      if (registry::isRegisteredType<T*>()) {
//        std::cout << "get, registered" << std::endl;
        wrapper<T*>* w = static_cast<wrapper<T*>*>(checkudata(L, index));
        return *w->ref;
      }
      throw std::runtime_error("trying to use unregistered type");
    }

    static int push(lua_State* L, T value) {
      if (registry::isRegisteredType<T*>()) {
//        std::cout << "push, registered" << std::endl;
        wrapper<T*>* w = wrapper<T*>::create(L);
        w->ref = new T(value);
        w->gc = true;
        luaL_getmetatable(L, registry::getTypeName<T*>().c_str());
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
    
    static boost::shared_ptr<T> get(lua_State* L, int index) {
      if (registry::isRegisteredType<T*>()) {
//        std::cout << "get, registered" << std::endl;
        wrapper<T*, boost::shared_ptr<T>*>* w = static_cast<wrapper<T*, boost::shared_ptr<T>*>*>(converter<T>::checkudata(L, index));
        return boost::shared_ptr<T>(*(w->holder));
      }
      throw std::runtime_error("trying to use unregistered type");
    }
    
    static int push(lua_State* L, const boost::shared_ptr<T>& value) {
      if (registry::isRegisteredType<T*>()) {
//        std::cout << "push, registered" << std::endl;
        wrapper<T*, boost::shared_ptr<T>*>* w = wrapper<T*, boost::shared_ptr<T>*>::create(L);
        w->holder = new boost::shared_ptr<T>(value);
        w->ref = value.get();
        w->gc = true;
        luaL_getmetatable(L, registry::getTypeName<T*>().c_str());
        lua_setmetatable(L, -2);
        return 1;
      }
      throw std::runtime_error("trying to use unregistered type");
    }

  };

  template<typename T>
  struct converter<const boost::shared_ptr<T>&> : converter<boost::shared_ptr<T> > {};
  
  template<typename T>
  struct converter<std::tr1::shared_ptr<T> > {
    
    static bool check(lua_State* L, int index) {
      return converter<T>::check(L, index);
    }
    
    static std::tr1::shared_ptr<T> get(lua_State* L, int index) {
      if (registry::isRegisteredType<T*>()) {
//        std::cout << "get, registered" << std::endl;
        wrapper<T*, std::tr1::shared_ptr<T>*>* w = static_cast<wrapper<T*, std::tr1::shared_ptr<T>*>*>(converter<T>::checkudata(L, index));
        return std::tr1::shared_ptr<T>(*(w->holder));
      }
      throw std::runtime_error("trying to use unregistered type");
    }
    
    static int push(lua_State* L, const std::tr1::shared_ptr<T>& value) {
      if (registry::isRegisteredType<T*>()) {
//        std::cout << "push, registered" << std::endl;
        wrapper<T*, std::tr1::shared_ptr<T>*>* w = wrapper<T*, std::tr1::shared_ptr<T>*>::create(L);
        w->holder = new std::tr1::shared_ptr<T>(value);
        w->ref = value.get();
        w->gc = true;
        luaL_getmetatable(L, registry::getTypeName<T*>().c_str());
        lua_setmetatable(L, -2);
        return 1;
      }
      throw std::runtime_error("trying to use unregistered type");
    }
    
  };
  
  template<typename T>
  struct converter<const std::tr1::shared_ptr<T>&> : converter<std::tr1::shared_ptr<T> > {};
  
  template<typename T>
  struct converter<T*> {
    
    static bool check(lua_State* L, int index) {
      return converter<T>::check(L, index);
    }

    static T* get(lua_State* L, int index) {
      if (registry::isRegisteredType<T*>()) {
//        std::cout << "get, registered" << std::endl;
        wrapper<T*>* w = static_cast<wrapper<T*>*>(converter<T>::checkudata(L, index));
        return w->ref;
      }
      throw std::runtime_error("trying to use unregistered type");
    }
    
    static int push(lua_State* L, T* value) {
      return push(L, value, false);
    }
    
    static int push(lua_State* L, T* value, bool gc) {
      if (registry::isRegisteredType<T*>()) {
//        std::cout << "push, registered" << std::endl;
        wrapper<T*>* w = wrapper<T*>::create(L);
        w->ref = value;
        w->gc = gc;
        luaL_getmetatable(L, registry::getTypeName<T*>().c_str());
        lua_setmetatable(L, -2);
        return 1;
      }
      throw std::runtime_error("trying to use unregistered type");
    }
    
  };
  
  template<typename T>
  struct converter<const T*> {
    
    static bool check(lua_State* L, int index) {
      return converter<T>::check(L, index);
    }
    
    static const T* get(lua_State* L, int index) {
      if (registry::isRegisteredType<T*>()) {
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
      if (registry::isRegisteredType<T*>()) {
//        std::cout << "push, registered" << std::endl;
        wrapper<const T*>* w = wrapper<const T*>::create(L);
        w->ref = value;
        w->gc = gc;
        luaL_getmetatable(L, registry::getTypeName<T*>().c_str());
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
      if (registry::isRegisteredType<T*>()) {
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
      if (registry::isRegisteredType<T*>()) {
//        std::cout << "push, registered" << std::endl;
        wrapper<T*>* w = wrapper<T*>::create(L);
        w->ref = &value;
        w->gc = gc;
        luaL_getmetatable(L, registry::getTypeName<T*>().c_str());
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
      if (registry::isRegisteredType<T*>()) {
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
      if (registry::isRegisteredType<T*>()) {
//        std::cout << "push, registered" << std::endl;
        wrapper<const T*>* w = wrapper<const T*>::create(L);
        w->ref = &value;
        w->gc = gc;
        luaL_getmetatable(L, registry::getTypeName<T*>().c_str());
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
  struct converter<std::string> {
    
    static bool check(lua_State* L, int index) {
      return lua_isstring(L, index);
    }
    
    static std::string get(lua_State* L, int index) {
      return luaL_checkstring(L, index);
    }
    
    static int push(lua_State* L, std::string value) {
      lua_pushstring(L, value.c_str());
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
  struct converter<const std::string&> {
    
    static bool check(lua_State* L, int index) {
      return lua_isstring(L, index);
    }
    
    static std::string get(lua_State* L, int index) {
      return luaL_checkstring(L, index);
    }
    
    static int push(lua_State* L, const std::string& value) {
      lua_pushstring(L, value.c_str());
      return 1;
    }
    
  };
  
  template<>
  struct converter<std::string&> {
    
    static bool check(lua_State* L, int index) {
      return lua_isstring(L, index);
    }
    
    static std::string get(lua_State* L, int index) {
      return luaL_checkstring(L, index);
    }
    
    static int push(lua_State* L, std::string& value) {
      lua_pushstring(L, value.c_str());
      return 1;
    }
    
  };
  
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
  
  template<>
  struct converter<const reference&> {
    
    static bool check(lua_State* L, int index) {
      return true;
    }
    
    static reference get(lua_State* L, int index) {
      return reference(L, index);
    }
    
    static int push(lua_State* L, const reference& ref) {
      return ref.push();
    }
    
  };
  
}

#endif
