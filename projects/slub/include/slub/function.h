#ifndef SLUB_FUNCTION_H
#define SLUB_FUNCTION_H

#include <map>
#include <string>

namespace slub {

  struct abstract_function_wrapper {
    virtual int call(lua_State* L) = 0;
  };

  struct function_holder {

    static function_holder instance;

    std::map<std::string, abstract_function_wrapper*> functions;

    ~function_holder() {
      std::cout << "cleanup functions" << std::endl;
      
      for (std::map<std::string, abstract_function_wrapper*>::iterator idx = functions.begin(); idx != functions.end(); ++idx) {
        delete idx->second;
      }
      functions.clear();
    }

    static void add(lua_State* L, const std::string& name, abstract_function_wrapper* f, int target) {
      instance.functions[name] = f;

      lua_pushlightuserdata(L, f);
      lua_pushcclosure(L, call, 1);
      lua_setfield(L, target != -1 ? target : LUA_GLOBALSINDEX, name.c_str());
    }

    static int call(lua_State* L) {
      abstract_function_wrapper* f = static_cast<abstract_function_wrapper*>(lua_touserdata(L, lua_upvalueindex(1)));
      return f->call(L);
    }

  };

  function_holder function_holder::instance;

  template<typename R, typename arg1, typename arg2, typename arg3>
  struct function_wrapper : public abstract_function_wrapper {
    
    R (*f)(arg1, arg2, arg3);
    
    function_wrapper(R (*f)(arg1, arg2, arg3)) : f(f) {
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f(converter<arg1>::get(L, -3), converter<arg2>::get(L, -2), converter<arg3>::get(L, -1)));
    }
    
  };
  
  template<>
  struct function_wrapper<void, empty, empty, empty> : public abstract_function_wrapper {
    
    void (*f)();
    
    function_wrapper(void (*f)()) : f(f) {
    }
    
    int call(lua_State* L) {
      f();
      return 0;
    }
    
  };

  template<typename arg1>
  struct function_wrapper<void, arg1, empty, empty> : public abstract_function_wrapper {
    
    void (*f)(arg1);
    
    function_wrapper(void (*f)(arg1)) : f(f) {
    }
    
    int call(lua_State* L) {
      f(converter<arg1>::get(L, -1));
      return 0;
    }
    
  };
  
  template<typename arg1, typename arg2>
  struct function_wrapper<void, arg1, arg2, empty> : public abstract_function_wrapper {
    
    void (*f)(arg1, arg2);
    
    function_wrapper(void (*f)(arg1, arg2)) : f(f) {
    }
    
    int call(lua_State* L) {
      f(converter<arg1>::get(L, -2), converter<arg2>::get(L, -1));
      return 0;
    }
    
  };
  
  template<typename arg1, typename arg2, typename arg3>
  struct function_wrapper<void, arg1, arg2, arg3> : public abstract_function_wrapper {
    
    void (*f)(arg1, arg2, arg3);
    
    function_wrapper(void (*f)(arg1, arg2, arg3)) : f(f) {
    }
    
    int call(lua_State* L) {
      f(converter<arg1>::get(L, -3), converter<arg2>::get(L, -2), converter<arg3>::get(L, -1));
      return 0;
    }
    
  };
  
  template<>
  struct function_wrapper<void, lua_State*, empty, empty> : public abstract_function_wrapper {
    
    void (*f)(lua_State*);
    
    function_wrapper(void (*f)(lua_State*)) : f(f) {
    }
    
    int call(lua_State* L) {
      f(L);
      return 0;
    }
    
  };
  
  template<typename arg1>
  struct function_wrapper<void, arg1, lua_State*, empty> : public abstract_function_wrapper {
    
    void (*f)(arg1, lua_State*);
    
    function_wrapper(void (*f)(arg1, lua_State*)) : f(f) {
    }
    
    int call(lua_State* L) {
      f(converter<arg1>::get(L, -1), L);
      return 0;
    }
    
  };
  
  template<typename arg1, typename arg2>
  struct function_wrapper<void, arg1, arg2, lua_State*> : public abstract_function_wrapper {
    
    void (*f)(arg1, arg2, lua_State*);
    
    function_wrapper(void (*f)(arg1, arg2, lua_State*)) : f(f) {
    }
    
    int call(lua_State* L) {
      f(converter<arg1>::get(L, -2), converter<arg2>::get(L, -1), L);
      return 0;
    }
    
  };
  
  template<typename R>
  struct function_wrapper<R, empty, empty, empty> : public abstract_function_wrapper {
    
    R (*f)();
    
    function_wrapper(R (*f)()) : f(f) {
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f());
    }
    
  };
  
  template<typename R, typename arg1>
  struct function_wrapper<R, arg1, empty, empty> : public abstract_function_wrapper {
    
    R (*f)(arg1);
    
    function_wrapper(R (*f)(arg1)) : f(f) {
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f(converter<arg1>::get(L, -1)));
    }
    
  };
  
  template<typename R, typename arg1, typename arg2>
  struct function_wrapper<R, arg1, arg2, empty> : public abstract_function_wrapper {
    
    R (*f)(arg1, arg2);
    
    function_wrapper(R (*f)(arg1, arg2)) : f(f) {
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f(converter<arg1>::get(L, -2), converter<arg2>::get(L, -1)));
    }
    
  };
  
  template<typename R>
  struct function_wrapper<R, lua_State*, empty, empty> : public abstract_function_wrapper {
    
    R (*f)(lua_State*);
    
    function_wrapper(R (*f)(lua_State*)) : f(f) {
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f(L));
    }
    
  };
  
  template<typename R, typename arg1>
  struct function_wrapper<R, arg1, lua_State*, empty> : public abstract_function_wrapper {
    
    R (*f)(arg1, lua_State*);
    
    function_wrapper(R (*f)(arg1, lua_State*)) : f(f) {
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f(converter<arg1>::get(L, -1), L));
    }
    
  };
  
  template<typename R, typename arg1, typename arg2>
  struct function_wrapper<R, arg1, arg2, lua_State*> : public abstract_function_wrapper {
    
    R (*f)(arg1, arg2, lua_State*);
    
    function_wrapper(R (*f)(arg1, arg2, lua_State*)) : f(f) {
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f(converter<arg1>::get(L, -2), converter<arg2>::get(L, -1), L));
    }
    
  };
  
  void function(lua_State* L, const std::string& name, void (*f)(), int target = -1) {
    function_holder::add(L, name, new function_wrapper<void, empty, empty, empty>(f), target);
  }

  template<typename arg1>
  void function(lua_State* L, const std::string& name, void (*f)(arg1), int target = -1) {
    function_holder::add(L, name, new function_wrapper<void, arg1, empty, empty>(f), target);
  }
  
  template<typename arg1, typename arg2>
  void function(lua_State* L, const std::string& name, void (*f)(arg1, arg2), int target = -1) {
    function_holder::add(L, name, new function_wrapper<void, arg1, arg2, empty>(f), target);
  }
  
  template<typename arg1, typename arg2, typename arg3>
  void function(lua_State* L, const std::string& name, void (*f)(arg1, arg2, arg3), int target = -1) {
    function_holder::add(L, name, new function_wrapper<void, arg1, arg2, arg3>(f), target);
  }
  
  template<typename R>
  void function(lua_State* L, const std::string& name, R (*f)(), int target = -1) {
    function_holder::add(L, name, new function_wrapper<R, empty, empty, empty>(f), target);
  }

  template<typename R, typename arg1>
  void function(lua_State* L, const std::string& name, R (*f)(arg1), int target = -1) {
    function_holder::add(L, name, new function_wrapper<R, arg1, empty, empty>(f), target);
  }
  
  template<typename R, typename arg1, typename arg2>
  void function(lua_State* L, const std::string& name, R (*f)(arg1, arg2), int target = -1) {
    function_holder::add(L, name, new function_wrapper<R, arg1, arg2, empty>(f), target);
  }
  
  template<typename R, typename arg1, typename arg2, typename arg3>
  void function(lua_State* L, const std::string& name, R (*f)(arg1, arg2, arg3), int target = -1) {
    function_holder::add(L, name, new function_wrapper<R, arg1, arg2, arg3>(f), target);
  }

}

#endif
