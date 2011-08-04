#ifndef SLUB_WRAPPER_H
#define SLUB_WRAPPER_H

namespace slub {

  template<typename T>
  struct empty_holder {
  };

  template<typename T, typename H = empty_holder<T>*>
  struct wrapper {
    H holder;
    T ref;
    bool gc;

    static wrapper* create(lua_State* L) {
      wrapper* w = (wrapper*) lua_newuserdata(L, sizeof(wrapper));
      w->holder = NULL;
      w->ref = NULL;
      w->gc = false;
      return w;
    }

  };

}

#endif
