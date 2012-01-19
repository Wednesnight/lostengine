/*
Copyright (c) 2011 Timo Boll, Tony Kostanjsek

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
