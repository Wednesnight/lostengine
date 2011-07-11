#include <iostream>
#include <iosfwd>

#include <slub/slub.h>
#include "foo.h"

int main (int argc, char * const argv[]) {

  lua_State *L = lua_open();

  luaopen_base(L);
  luaopen_table(L);
  luaopen_string(L);
  luaopen_math(L);
  luaopen_debug(L);

  slub::clazz<foo>("foo", L)
    .field("bar", &foo::bar)
    .field("f", &foo::f)
    .method("doStuff", &foo::doStuff)
    .method("doStuff2", &foo::doStuff2)
    .method("getFoo", &foo::getFoo)
    .tostring()
    .add<int, int>()
    .sub<int, int>()
    .mul<int, int>()
    .div<int, int>()
    .mod<int, int>()
    .pow<int, int>()
    .unm<int>()
    .lt()
    .le();

  if (luaL_dofile(L, "foo.lua")) {
    std::cout << lua_tostring(L, -1) << std::endl;
  }

  slub::clazz<std::string>("str", L);

  luaL_dostring(L, "local s = str()");

  lua_gc(L, LUA_GCCOLLECT, 0);
  lua_close(L);

  return 0;
}
