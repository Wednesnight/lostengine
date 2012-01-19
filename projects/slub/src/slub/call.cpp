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

#include "../../include/slub/call.h"

#include <stdexcept>

namespace slub {

  static lua_CFunction default_error_handler = NULL;

  void set_error_handler(lua_CFunction error_handler) {
    default_error_handler = error_handler;
  }

  // traceback function from lua.c
  int traceback (lua_State *L)
  {
    // look up Lua's 'debug.traceback' function
    lua_getglobal(L, "debug");
    if (!lua_istable(L, -1))
    {
      lua_pop(L, 1);
      return 1;
    }
    lua_getfield(L, -1, "traceback");
    if (!lua_isfunction(L, -1))
    {
      lua_pop(L, 2);
      return 1;
    }
    lua_pushvalue(L, 1);  /* pass error message */
    lua_pushinteger(L, 2);  /* skip this function and traceback */
    lua_call(L, 2, 1);  /* call debug.traceback */
    return lua_gettop(L);
  }

  int error_handler(lua_State* state) {
    int result = 1;
    if (default_error_handler == NULL || (result = default_error_handler(state)) == 0) {
      throw std::runtime_error(lua_tostring(state, traceback(state)));
    }
    return result;
  }

  int call(lua_State* state, int nargs, int nresults) {
    return pcall(state, nargs, nresults, error_handler);
  }

  int pcall(lua_State* state, int nargs, int nresults, lua_CFunction errfunc) {
    int errfunc_idx = lua_gettop(state) - nargs;
    lua_pushcfunction(state, errfunc);
    lua_insert(state, errfunc_idx);
    int result = lua_pcall(state, nargs, nresults, errfunc_idx);
    lua_remove(state, errfunc_idx);
    return result;
  }

}
