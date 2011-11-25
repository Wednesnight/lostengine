#ifndef SLUB_FUNCTION_H
#define SLUB_FUNCTION_H

#include "config.h"
#include "slub_lua.h"
#include "converter.h"

#include <map>
#include <list>

namespace slub {

  struct abstract_function_wrapper {
    virtual bool check(lua_State* L) = 0;
    virtual int call(lua_State* L) = 0;
  };
  
  struct function_holder {
    
    static function_holder instance;
    std::map<string, std::list<abstract_function_wrapper*> > functions;
    
    ~function_holder();
    static void add(lua_State* L, const string& name, abstract_function_wrapper* f, const string& prefix, int target);
    static int call(lua_State* L);
    
  };

  template<typename R, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6, typename arg7>
  struct function_wrapper : public abstract_function_wrapper {
    
    R (*f)(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    
    function_wrapper(R (*f)(arg1, arg2, arg3, arg4, arg5, arg6, arg7)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 7 && converter<arg1>::check(L, -7) && converter<arg2>::check(L, -6) && converter<arg3>::check(L, -5)
          && converter<arg4>::check(L, -4) && converter<arg5>::check(L, -3) && converter<arg6>::check(L, -2) && converter<arg7>::check(L, -1);
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f(converter<arg1>::get(L, -7), converter<arg2>::get(L, -6), converter<arg3>::get(L, -5),
                                     converter<arg4>::get(L, -4), converter<arg5>::get(L, -3), converter<arg6>::get(L, -2),
                                     converter<arg7>::get(L, -1)));
    }
    
  };
  
  template<>
  struct function_wrapper<void, empty, empty, empty, empty, empty, empty, empty> : public abstract_function_wrapper {
    
    void (*f)();
    
    function_wrapper(void (*f)()) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 0;
    }
    
    int call(lua_State* L) {
      f();
      return 0;
    }
    
  };

  template<typename arg1>
  struct function_wrapper<void, arg1, empty, empty, empty, empty, empty, empty> : public abstract_function_wrapper {
    
    void (*f)(arg1);
    
    function_wrapper(void (*f)(arg1)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 1 && converter<arg1>::check(L, -1);
    }
    
    int call(lua_State* L) {
      f(converter<arg1>::get(L, -1));
      return 0;
    }
    
  };
  
  template<typename arg1, typename arg2>
  struct function_wrapper<void, arg1, arg2, empty, empty, empty, empty, empty> : public abstract_function_wrapper {
    
    void (*f)(arg1, arg2);
    
    function_wrapper(void (*f)(arg1, arg2)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 2 && converter<arg1>::check(L, -2) && converter<arg2>::check(L, -1);
    }
    
    int call(lua_State* L) {
      f(converter<arg1>::get(L, -2), converter<arg2>::get(L, -1));
      return 0;
    }
    
  };
  
  template<typename arg1, typename arg2, typename arg3>
  struct function_wrapper<void, arg1, arg2, arg3, empty, empty, empty, empty> : public abstract_function_wrapper {
    
    void (*f)(arg1, arg2, arg3);
    
    function_wrapper(void (*f)(arg1, arg2, arg3)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 3 && converter<arg1>::check(L, -3) && converter<arg2>::check(L, -2) && converter<arg3>::check(L, -1);
    }
    
    int call(lua_State* L) {
      f(converter<arg1>::get(L, -3), converter<arg2>::get(L, -2), converter<arg3>::get(L, -1));
      return 0;
    }
    
  };
  
  template<typename arg1, typename arg2, typename arg3, typename arg4>
  struct function_wrapper<void, arg1, arg2, arg3, arg4, empty, empty, empty> : public abstract_function_wrapper {
    
    void (*f)(arg1, arg2, arg3, arg4);
    
    function_wrapper(void (*f)(arg1, arg2, arg3, arg4)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 4 && converter<arg1>::check(L, -4) && converter<arg2>::check(L, -3) && converter<arg3>::check(L, -2)
           && converter<arg4>::check(L, -1);
    }
    
    int call(lua_State* L) {
      f(converter<arg1>::get(L, -4), converter<arg2>::get(L, -3), converter<arg3>::get(L, -2), converter<arg4>::get(L, -1));
      return 0;
    }
    
  };
  
  template<typename arg1, typename arg2, typename arg3, typename arg4, typename arg5>
  struct function_wrapper<void, arg1, arg2, arg3, arg4, arg5, empty, empty> : public abstract_function_wrapper {
    
    void (*f)(arg1, arg2, arg3, arg4, arg5);
    
    function_wrapper(void (*f)(arg1, arg2, arg3, arg4, arg5)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 5 && converter<arg1>::check(L, -5) && converter<arg2>::check(L, -4) && converter<arg3>::check(L, -3)
           && converter<arg4>::check(L, -2) && converter<arg5>::check(L, -1);
    }
    
    int call(lua_State* L) {
      f(converter<arg1>::get(L, -5), converter<arg2>::get(L, -4), converter<arg3>::get(L, -3), converter<arg4>::get(L, -2),
        converter<arg5>::get(L, -1));
      return 0;
    }
    
  };
  
  template<typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6>
  struct function_wrapper<void, arg1, arg2, arg3, arg4, arg5, arg6, empty> : public abstract_function_wrapper {
    
    void (*f)(arg1, arg2, arg3, arg4, arg5, arg6);
    
    function_wrapper(void (*f)(arg1, arg2, arg3, arg4, arg5, arg6)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 6 && converter<arg1>::check(L, -6) && converter<arg2>::check(L, -5) && converter<arg3>::check(L, -4)
      && converter<arg4>::check(L, -3) && converter<arg5>::check(L, -2) && converter<arg6>::check(L, -1);
    }
    
    int call(lua_State* L) {
      f(converter<arg1>::get(L, -6), converter<arg2>::get(L, -5), converter<arg3>::get(L, -4), converter<arg4>::get(L, -3),
        converter<arg5>::get(L, -2), converter<arg6>::get(L, -1));
      return 0;
    }
    
  };
  
  template<typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6, typename arg7>
  struct function_wrapper<void, arg1, arg2, arg3, arg4, arg5, arg6, arg7> : public abstract_function_wrapper {
    
    void (*f)(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    
    function_wrapper(void (*f)(arg1, arg2, arg3, arg4, arg5, arg6, arg7)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 7 && converter<arg1>::check(L, -7) && converter<arg2>::check(L, -6) && converter<arg3>::check(L, -5)
      && converter<arg4>::check(L, -4) && converter<arg5>::check(L, -3) && converter<arg6>::check(L, -2) && converter<arg7>::check(L, -1);
    }
    
    int call(lua_State* L) {
      f(converter<arg1>::get(L, -7), converter<arg2>::get(L, -6), converter<arg3>::get(L, -5), converter<arg4>::get(L, -4),
        converter<arg5>::get(L, -3), converter<arg6>::get(L, -2), converter<arg7>::get(L, -1));
      return 0;
    }
    
  };
  
  template<>
  struct function_wrapper<void, lua_State*, empty, empty, empty, empty, empty, empty> : public abstract_function_wrapper {
    
    void (*f)(lua_State*);
    
    function_wrapper(void (*f)(lua_State*)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 0;
    }
    
    int call(lua_State* L) {
      f(L);
      return 0;
    }
    
  };
  
  template<typename arg1>
  struct function_wrapper<void, arg1, lua_State*, empty, empty, empty, empty, empty> : public abstract_function_wrapper {
    
    void (*f)(arg1, lua_State*);
    
    function_wrapper(void (*f)(arg1, lua_State*)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 1 && converter<arg1>::check(L, -1);
    }
    
    int call(lua_State* L) {
      f(converter<arg1>::get(L, -1), L);
      return 0;
    }
    
  };
  
  template<typename arg1, typename arg2>
  struct function_wrapper<void, arg1, arg2, lua_State*, empty, empty, empty, empty> : public abstract_function_wrapper {
    
    void (*f)(arg1, arg2, lua_State*);
    
    function_wrapper(void (*f)(arg1, arg2, lua_State*)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 2 && converter<arg1>::check(L, -2) && converter<arg2>::check(L, -1);
    }
    
    int call(lua_State* L) {
      f(converter<arg1>::get(L, -2), converter<arg2>::get(L, -1), L);
      return 0;
    }
    
  };
  
  template<typename arg1, typename arg2, typename arg3>
  struct function_wrapper<void, arg1, arg2, arg3, lua_State*, empty, empty, empty> : public abstract_function_wrapper {
    
    void (*f)(arg1, arg2, arg3, lua_State*);
    
    function_wrapper(void (*f)(arg1, arg2, arg3, lua_State*)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 3 && converter<arg1>::check(L, -3) && converter<arg2>::check(L, -2) && converter<arg3>::check(L, -1);
    }
    
    int call(lua_State* L) {
      f(converter<arg1>::get(L, -3), converter<arg2>::get(L, -2), converter<arg3>::get(L, -1), L);
      return 0;
    }
    
  };
  
  template<typename arg1, typename arg2, typename arg3, typename arg4>
  struct function_wrapper<void, arg1, arg2, arg3, arg4, lua_State*, empty, empty> : public abstract_function_wrapper {
    
    void (*f)(arg1, arg2, arg3, arg4, lua_State*);
    
    function_wrapper(void (*f)(arg1, arg2, arg3, arg4, lua_State*)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 4 && converter<arg1>::check(L, -4) && converter<arg2>::check(L, -3) && converter<arg3>::check(L, -2)
           && converter<arg4>::check(L, -1);
    }
    
    int call(lua_State* L) {
      f(converter<arg1>::get(L, -4), converter<arg2>::get(L, -3), converter<arg3>::get(L, -2), converter<arg4>::get(L, -1), L);
      return 0;
    }
    
  };
  
  template<typename arg1, typename arg2, typename arg3, typename arg4, typename arg5>
  struct function_wrapper<void, arg1, arg2, arg3, arg4, arg5, lua_State*, empty> : public abstract_function_wrapper {
    
    void (*f)(arg1, arg2, arg3, arg4, arg5, lua_State*);
    
    function_wrapper(void (*f)(arg1, arg2, arg3, arg4, arg5, lua_State*)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 5 && converter<arg1>::check(L, -5) && converter<arg2>::check(L, -4) && converter<arg3>::check(L, -3)
      && converter<arg4>::check(L, -2) && converter<arg5>::check(L, -1);
    }
    
    int call(lua_State* L) {
      f(converter<arg1>::get(L, -5), converter<arg2>::get(L, -4), converter<arg3>::get(L, -3), converter<arg4>::get(L, -2),
        converter<arg5>::get(L, -1), L);
      return 0;
    }
    
  };
  
  template<typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6>
  struct function_wrapper<void, arg1, arg2, arg3, arg4, arg5, arg6, lua_State*> : public abstract_function_wrapper {
    
    void (*f)(arg1, arg2, arg3, arg4, arg5, arg6, lua_State*);
    
    function_wrapper(void (*f)(arg1, arg2, arg3, arg4, arg5, arg6, lua_State*)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 6 && converter<arg1>::check(L, -6) && converter<arg2>::check(L, -5) && converter<arg3>::check(L, -4)
      && converter<arg4>::check(L, -3) && converter<arg5>::check(L, -2) && converter<arg6>::check(L, -1);
    }
    
    int call(lua_State* L) {
      f(converter<arg1>::get(L, -6), converter<arg2>::get(L, -5), converter<arg3>::get(L, -4), converter<arg4>::get(L, -3),
        converter<arg5>::get(L, -2), converter<arg6>::get(L, -1), L);
      return 0;
    }
    
  };
  
  template<typename R>
  struct function_wrapper<R, empty, empty, empty, empty, empty, empty, empty> : public abstract_function_wrapper {
    
    R (*f)();
    
    function_wrapper(R (*f)()) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 0;
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f());
    }
    
  };
  
  template<typename R, typename arg1>
  struct function_wrapper<R, arg1, empty, empty, empty, empty, empty, empty> : public abstract_function_wrapper {
    
    R (*f)(arg1);
    
    function_wrapper(R (*f)(arg1)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 1 && converter<arg1>::check(L, -1);
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f(converter<arg1>::get(L, -1)));
    }
    
  };
  
  template<typename R, typename arg1, typename arg2>
  struct function_wrapper<R, arg1, arg2, empty, empty, empty, empty, empty> : public abstract_function_wrapper {
    
    R (*f)(arg1, arg2);
    
    function_wrapper(R (*f)(arg1, arg2)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 2 && converter<arg1>::check(L, -2) && converter<arg2>::check(L, -1);
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f(converter<arg1>::get(L, -2), converter<arg2>::get(L, -1)));
    }
    
  };
  
  template<typename R, typename arg1, typename arg2, typename arg3>
  struct function_wrapper<R, arg1, arg2, arg3, empty, empty, empty, empty> : public abstract_function_wrapper {
    
    R (*f)(arg1, arg2, arg3);
    
    function_wrapper(R (*f)(arg1, arg2, arg3)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 3 && converter<arg1>::check(L, -3) && converter<arg2>::check(L, -2) && converter<arg3>::check(L, -1);
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f(converter<arg1>::get(L, -3), converter<arg2>::get(L, -2), converter<arg3>::get(L, -1)));
    }
    
  };
  
  template<typename R, typename arg1, typename arg2, typename arg3, typename arg4>
  struct function_wrapper<R, arg1, arg2, arg3, arg4, empty, empty, empty> : public abstract_function_wrapper {
    
    R (*f)(arg1, arg2, arg3, arg4);
    
    function_wrapper(R (*f)(arg1, arg2, arg3, arg4)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 4 && converter<arg1>::check(L, -4) && converter<arg2>::check(L, -3) && converter<arg3>::check(L, -2)
           && converter<arg4>::check(L, -1);
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f(converter<arg1>::get(L, -4), converter<arg2>::get(L, -3), converter<arg3>::get(L, -2),
                                     converter<arg4>::get(L, -1)));
    }
    
  };
  
  template<typename R, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5>
  struct function_wrapper<R, arg1, arg2, arg3, arg4, arg5, empty, empty> : public abstract_function_wrapper {
    
    R (*f)(arg1, arg2, arg3, arg4, arg5);
    
    function_wrapper(R (*f)(arg1, arg2, arg3, arg4, arg5)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 5 && converter<arg1>::check(L, -5) && converter<arg2>::check(L, -4) && converter<arg3>::check(L, -3)
      && converter<arg4>::check(L, -2) && converter<arg5>::check(L, -1);
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f(converter<arg1>::get(L, -5), converter<arg2>::get(L, -4), converter<arg3>::get(L, -3),
                                     converter<arg4>::get(L, -2), converter<arg5>::get(L, -1)));
    }
    
  };
  
  template<typename R, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6>
  struct function_wrapper<R, arg1, arg2, arg3, arg4, arg5, arg6, empty> : public abstract_function_wrapper {
    
    R (*f)(arg1, arg2, arg3, arg4, arg5, arg6);
    
    function_wrapper(R (*f)(arg1, arg2, arg3, arg4, arg5, arg6)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 6 && converter<arg1>::check(L, -6) && converter<arg2>::check(L, -5) && converter<arg3>::check(L, -4)
      && converter<arg4>::check(L, -3) && converter<arg5>::check(L, -2) && converter<arg6>::check(L, -1);
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f(converter<arg1>::get(L, -6), converter<arg2>::get(L, -5), converter<arg3>::get(L, -4),
                                     converter<arg4>::get(L, -3), converter<arg5>::get(L, -2), converter<arg6>::get(L, -1)));
    }
    
  };
  
  template<typename R>
  struct function_wrapper<R, lua_State*, empty, empty, empty, empty, empty, empty> : public abstract_function_wrapper {
    
    R (*f)(lua_State*);
    
    function_wrapper(R (*f)(lua_State*)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 0;
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f(L));
    }
    
  };
  
  template<typename R, typename arg1>
  struct function_wrapper<R, arg1, lua_State*, empty, empty, empty, empty, empty> : public abstract_function_wrapper {
    
    R (*f)(arg1, lua_State*);
    
    function_wrapper(R (*f)(arg1, lua_State*)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 1 && converter<arg1>::check(L, -1);
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f(converter<arg1>::get(L, -1), L));
    }
    
  };
  
  template<typename R, typename arg1, typename arg2>
  struct function_wrapper<R, arg1, arg2, lua_State*, empty, empty, empty, empty> : public abstract_function_wrapper {
    
    R (*f)(arg1, arg2, lua_State*);
    
    function_wrapper(R (*f)(arg1, arg2, lua_State*)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 2 && converter<arg1>::check(L, -2) && converter<arg2>::check(L, -1);
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f(converter<arg1>::get(L, -2), converter<arg2>::get(L, -1), L));
    }
    
  };
  
  template<typename R, typename arg1, typename arg2, typename arg3>
  struct function_wrapper<R, arg1, arg2, arg3, lua_State*, empty, empty, empty> : public abstract_function_wrapper {
    
    R (*f)(arg1, arg2, arg3, lua_State*);
    
    function_wrapper(R (*f)(arg1, arg2, arg3, lua_State*)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 3 && converter<arg1>::check(L, -3) && converter<arg2>::check(L, -2) && converter<arg3>::check(L, -1);
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f(converter<arg1>::get(L, -3), converter<arg2>::get(L, -2), converter<arg3>::get(L, -1), L));
    }
    
  };
  
  template<typename R, typename arg1, typename arg2, typename arg3, typename arg4>
  struct function_wrapper<R, arg1, arg2, arg3, arg4, lua_State*, empty, empty> : public abstract_function_wrapper {
    
    R (*f)(arg1, arg2, arg3, arg4, lua_State*);
    
    function_wrapper(R (*f)(arg1, arg2, arg3, arg4, lua_State*)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 4 && converter<arg1>::check(L, -4) && converter<arg2>::check(L, -3) && converter<arg3>::check(L, -2)
           && converter<arg4>::check(L, -1);
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f(converter<arg1>::get(L, -4), converter<arg2>::get(L, -3), converter<arg3>::get(L, -2),
                                     converter<arg4>::get(L, -1), L));
    }
    
  };
  
  template<typename R, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5>
  struct function_wrapper<R, arg1, arg2, arg3, arg4, arg5, lua_State*, empty> : public abstract_function_wrapper {
    
    R (*f)(arg1, arg2, arg3, arg4, arg5, lua_State*);
    
    function_wrapper(R (*f)(arg1, arg2, arg3, arg4, arg5, lua_State*)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 5 && converter<arg1>::check(L, -5) && converter<arg2>::check(L, -4) && converter<arg3>::check(L, -3)
      && converter<arg4>::check(L, -2) && converter<arg5>::check(L, -1);
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f(converter<arg1>::get(L, -5), converter<arg2>::get(L, -4), converter<arg3>::get(L, -3),
                                     converter<arg4>::get(L, -2), converter<arg5>::get(L, -1), L));
    }
    
  };
  
  template<typename R, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6>
  struct function_wrapper<R, arg1, arg2, arg3, arg4, arg5, arg6, lua_State*> : public abstract_function_wrapper {
    
    R (*f)(arg1, arg2, arg3, arg4, arg5, arg6, lua_State*);
    
    function_wrapper(R (*f)(arg1, arg2, arg3, arg4, arg5, arg6, lua_State*)) : f(f) {
    }
    
    bool check(lua_State* L) {
      return lua_gettop(L) == 6 && converter<arg1>::check(L, -6) && converter<arg2>::check(L, -5) && converter<arg3>::check(L, -4)
      && converter<arg4>::check(L, -3) && converter<arg5>::check(L, -2) && converter<arg6>::check(L, -1);
    }
    
    int call(lua_State* L) {
      return converter<R>::push(L, f(converter<arg1>::get(L, -6), converter<arg2>::get(L, -5), converter<arg3>::get(L, -4),
                                     converter<arg4>::get(L, -3), converter<arg5>::get(L, -2), converter<arg6>::get(L, -1), L));
    }
    
  };
  
  static void function(lua_State* L, const string& name, void (*f)(), const string& prefix = "", int target = -1) {
    function_holder::add(L, name, new function_wrapper<void, empty, empty, empty, empty, empty, empty, empty>(f), prefix, target);
  }
  
  template<typename arg1>
  static void function(lua_State* L, const string& name, void (*f)(arg1), const string& prefix = "", int target = -1) {
    function_holder::add(L, name, new function_wrapper<void, arg1, empty, empty, empty, empty, empty, empty>(f), prefix, target);
  }
  
  template<typename arg1, typename arg2>
  static void function(lua_State* L, const string& name, void (*f)(arg1, arg2), const string& prefix = "", int target = -1) {
    function_holder::add(L, name, new function_wrapper<void, arg1, arg2, empty, empty, empty, empty, empty>(f), prefix, target);
  }
  
  template<typename arg1, typename arg2, typename arg3>
  static void function(lua_State* L, const string& name, void (*f)(arg1, arg2, arg3), const string& prefix = "", int target = -1) {
    function_holder::add(L, name, new function_wrapper<void, arg1, arg2, arg3, empty, empty, empty, empty>(f), prefix, target);
  }
  
  template<typename arg1, typename arg2, typename arg3, typename arg4>
  static void function(lua_State* L, const string& name, void (*f)(arg1, arg2, arg3, arg4), const string& prefix = "", int target = -1) {
    function_holder::add(L, name, new function_wrapper<void, arg1, arg2, arg3, arg4, empty, empty, empty>(f), prefix, target);
  }
  
  template<typename arg1, typename arg2, typename arg3, typename arg4, typename arg5>
  static void function(lua_State* L, const string& name, void (*f)(arg1, arg2, arg3, arg4, arg5), const string& prefix = "", int target = -1) {
    function_holder::add(L, name, new function_wrapper<void, arg1, arg2, arg3, arg4, arg5, empty, empty>(f), prefix, target);
  }
  
  template<typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6>
  static void function(lua_State* L, const string& name, void (*f)(arg1, arg2, arg3, arg4, arg5, arg6), const string& prefix = "", int target = -1) {
    function_holder::add(L, name, new function_wrapper<void, arg1, arg2, arg3, arg4, arg5, arg6, empty>(f), prefix, target);
  }
  
  template<typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6, typename arg7>
  static void function(lua_State* L, const string& name, void (*f)(arg1, arg2, arg3, arg4, arg5, arg6, arg7), const string& prefix = "", int target = -1) {
    function_holder::add(L, name, new function_wrapper<void, arg1, arg2, arg3, arg4, arg5, arg6, arg7>(f), prefix, target);
  }
  
  template<typename R>
  static void function(lua_State* L, const string& name, R (*f)(), const string& prefix = "", int target = -1) {
    function_holder::add(L, name, new function_wrapper<R, empty, empty, empty, empty, empty, empty, empty>(f), prefix, target);
  }
  
  template<typename R, typename arg1>
  static void function(lua_State* L, const string& name, R (*f)(arg1), const string& prefix = "", int target = -1) {
    function_holder::add(L, name, new function_wrapper<R, arg1, empty, empty, empty, empty, empty, empty>(f), prefix, target);
  }
  
  template<typename R, typename arg1, typename arg2>
  static void function(lua_State* L, const string& name, R (*f)(arg1, arg2), const string& prefix = "", int target = -1) {
    function_holder::add(L, name, new function_wrapper<R, arg1, arg2, empty, empty, empty, empty, empty>(f), prefix, target);
  }
  
  template<typename R, typename arg1, typename arg2, typename arg3>
  static void function(lua_State* L, const string& name, R (*f)(arg1, arg2, arg3), const string& prefix = "", int target = -1) {
    function_holder::add(L, name, new function_wrapper<R, arg1, arg2, arg3, empty, empty, empty, empty>(f), prefix, target);
  }
  
  template<typename R, typename arg1, typename arg2, typename arg3, typename arg4>
  static void function(lua_State* L, const string& name, R (*f)(arg1, arg2, arg3, arg4), const string& prefix = "", int target = -1) {
    function_holder::add(L, name, new function_wrapper<R, arg1, arg2, arg3, arg4, empty, empty, empty>(f), prefix, target);
  }
  
  template<typename R, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5>
  static void function(lua_State* L, const string& name, R (*f)(arg1, arg2, arg3, arg4, arg5), const string& prefix = "", int target = -1) {
    function_holder::add(L, name, new function_wrapper<R, arg1, arg2, arg3, arg4, arg5, empty, empty>(f), prefix, target);
  }
  
  template<typename R, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6>
  static void function(lua_State* L, const string& name, R (*f)(arg1, arg2, arg3, arg4, arg5, arg6), const string& prefix = "", int target = -1) {
    function_holder::add(L, name, new function_wrapper<R, arg1, arg2, arg3, arg4, arg5, arg6, empty>(f), prefix, target);
  }
  
  template<typename R, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6, typename arg7>
  static void function(lua_State* L, const string& name, R (*f)(arg1, arg2, arg3, arg4, arg5, arg6, arg7), const string& prefix = "", int target = -1) {
    function_holder::add(L, name, new function_wrapper<R, arg1, arg2, arg3, arg4, arg5, arg6, arg7>(f), prefix, target);
  }
  
}

#endif
