#ifndef SLUB_FUNCTION_H
#define SLUB_FUNCTION_H

#include <map>
#include <string>

namespace slub {

  struct abstract_function_wrapper {
    virtual int call(lua_State* L) = 0;
  };

  template<typename R, typename arg1, typename arg2, typename arg3>
  struct function_wrapper;

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

    static void add(const std::string& name, abstract_function_wrapper* f) {
      instance.functions[name] = f;
    }

  };

  function_holder function_holder::instance;

  template<typename R = void, typename arg1 = empty, typename arg2 = empty, typename arg3 = empty>
  struct function {

    lua_State* state;

    function(lua_State* L, const std::string& name, R (*f)(arg1, arg2, arg3), int target = -1) : state(L) {
      abstract_function_wrapper* w = new function_wrapper<R, arg1, arg2, arg3>(f);
      function_holder::add(name, w);

      lua_pushstring(L, name.c_str());
      lua_pushlightuserdata(L, w);
      lua_pushcclosure(L, call, 1);
      lua_settable(L, target != -1 ? target : LUA_GLOBALSINDEX);
    }

    static int call(lua_State* L) {
      abstract_function_wrapper* w = static_cast<abstract_function_wrapper*>(lua_touserdata(L, lua_upvalueindex(1)));
      return w->call(L);
    }

  };

  template<typename R = void, typename arg1 = empty, typename arg2 = empty, typename arg3 = empty>
  struct function_wrapper : public abstract_function_wrapper {

    R (*f)(arg1, arg2, arg3);

    function_wrapper(R (*f)(arg1, arg2, arg3)) : f(f) {
    }

    int call(lua_State* L) {
      converter<R>::push(L, f(converter<arg1>::get(L, -2), converter<arg1>::get(L, -1), converter<arg1>::get(L, 1)));
      return 1;
    }

  };

}

#endif
