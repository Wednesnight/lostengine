#ifndef SLUB_CALL_H
#define SLUB_CALL_H

#include "config.h"
#include "slub_lua.h"

namespace slub {

  void set_error_handler(lua_CFunction error_handler);

  int call(lua_State* state, int nargs, int nresults);
  int pcall(lua_State* state, int nargs, int nresults, lua_CFunction errfunc);

}

#endif
