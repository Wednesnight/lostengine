#include "../../include/slub/call.h"

#include <stdexcept>

namespace slub {

  static lua_CFunction default_error_handler = NULL;

  void set_error_handler(lua_CFunction error_handler) {
    default_error_handler = error_handler;
  }

  static int error_handler(lua_State* state) {
    int result = 1;
    if (default_error_handler == NULL || (result = default_error_handler(state)) == 0) {
      throw std::runtime_error(lua_tostring(state, -1));
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
