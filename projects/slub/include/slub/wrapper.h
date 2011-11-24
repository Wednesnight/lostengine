#ifndef SLUB_WRAPPER_H
#define SLUB_WRAPPER_H

#include "slub_lua.h"

#include <typeinfo>

namespace slub {

  template<typename T>
  struct empty_holder {
  };

  struct wrapper_base {
    const std::type_info* type;
  };

  template<typename T, typename H = empty_holder<T>*>
  struct wrapper : public wrapper_base {
    H holder;
    T ref;
    bool gc;

    static wrapper* create(lua_State* L, const std::type_info& type) {
      wrapper* w = (wrapper*) lua_newuserdata(L, sizeof(wrapper));
      w->type = &type;
      w->holder = NULL;
      w->ref = NULL;
      w->gc = false;
      return w;
    }

  };

}

#endif
