#include <iostream>
#include <iosfwd>

#include <slub/slub.h>
#include "foo.h"

#include <lost/math/Vec2.h>
#include <lost/math/io.h>

using namespace lost::math;

namespace slub {

  template<>
  struct constructor<foo> {
    static foo* newInstance(lua_State* L) {
      return lua_gettop(L) == 2 ? new foo(luaL_checknumber(L, -1)) : new foo(luaL_checknumber(L, -2), luaL_checknumber(L, -1));
    }
  };

  template<>
  struct constructor<Vec2> {
    static Vec2* newInstance(lua_State* L) {
      if (lua_gettop(L) == 3) {
        return new Vec2(luaL_checknumber(L, -2), luaL_checknumber(L, -1));
      }
      else if (lua_gettop(L) == 2) {
        return new Vec2(*converter<Vec2*>::get(L, -1));
      }
      else {
        return new Vec2();
      }
    }
  };
  
  template<>
  struct converter<Vec2> {
    
    static Vec2& get(lua_State* L, int index) {
      return *converter<Vec2*>::get(L, index);
    }
    
    static int push(lua_State* L, Vec2 value) {
      return converter<Vec2*>::push(L, &value);
    }
    
    static int push(lua_State* L, float value) {
      return converter<float>::push(L, value);
    }
    
  };
  
}

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

  // TODO: namespaces

  slub::clazz<Vec2>("Vec2", L)
  .method("zero", &Vec2::zero)
  .field("x", &Vec2::x)
  .field("y", &Vec2::y)
  .field("width", &Vec2::width)
  .field("height", &Vec2::height)
  .mul<Vec2, float>()
  .mul<Vec2, Vec2>()
  .add<Vec2, Vec2>()
  .sub<Vec2, Vec2>();

  // TODO: external operators

//  .tostring();

  luaL_dostring(L, "local v2 = Vec2() local vec2 = Vec2(10, 10) local vec2_2 = Vec2(vec2) vec2_2.x = 11 vec2_2.y = 11 print(vec2 * vec2_2) print(vec2 - vec2_2)");

  // TODO: simple functions

/*
  def("len", (float(*)(const Vec2&))&len)
  def("squlen", (float(*)(const Vec2&))&squlen)
  def("normalise", (Vec2&(*)(Vec2&))&normalise)
  def("perpendicular", (Vec2(*)(Vec2&))&perpendicular)
  def("angle", (float(*)(const Vec2&, const Vec2&))&angle)
  def("compare", (bool(*)(const Vec2&, const Vec2&, float))&compare)
*/
  
  lua_gc(L, LUA_GCCOLLECT, 0);
  lua_close(L);

  return 0;
}
