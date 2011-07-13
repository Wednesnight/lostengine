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
    
    static Vec2 get(lua_State* L, int index) {
      return *converter<Vec2*>::get(L, index);
    }
    
    static int push(lua_State* L, Vec2 value) {
      return converter<Vec2*>::push(L, new Vec2(value), true);
    }
    
    static int push(lua_State* L, float value) {
      return converter<float>::push(L, value);
    }
    
  };
  
  template<>
  struct converter<const Vec2&> {
    
    static const Vec2& get(lua_State* L, int index) {
      return *converter<Vec2*>::get(L, index);
    }
    
    static int push(lua_State* L, const Vec2& value) {
      return converter<Vec2*>::push(L, new Vec2(value), true);
    }
    
  };
  
  template<>
  struct converter<Vec2&> {
    
    static Vec2& get(lua_State* L, int index) {
      return *converter<Vec2*>::get(L, index);
    }
    
    static int push(lua_State* L, Vec2& value) {
      return converter<Vec2*>::push(L, new Vec2(value), true);
    }
    
  };
  
}

void testing0() {
  std::cout << "testing0()" << std::endl;
}

void testing1(int i) {
  std::cout << "testing1(" << i << ")" << std::endl;
}

void testing2(int i, int j) {
  std::cout << "testing2(" << i << ", " << j << ")" << std::endl;
}

void testing3(int i, int j, int k) {
  std::cout << "testing3(" << i << ", " << j << ", " << k << ")" << std::endl;
}

int testing(int i, int j, int k) {
  return i+j+k;
}

int main (int argc, char * const argv[]) {

  lua_State *L = lua_open();

  luaopen_base(L);
  luaopen_table(L);
  luaopen_string(L);
  luaopen_math(L);
  luaopen_debug(L);

  slub::clazz<foo>(L, "foo")
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

  slub::clazz<std::string>(L, "str");

  if (luaL_dostring(L, "local s = str()")) {
    std::cout << lua_tostring(L, -1) << std::endl;
  }

  slub::package(L, "lost").package("math").clazz<Vec2>("Vec2")
// TODO: method overloading ?
    .method("zero", &Vec2::zero)
    .field("x", &Vec2::x)
    .field("y", &Vec2::y)
    .field("width", &Vec2::width)
    .field("height", &Vec2::height)
    .mul<Vec2, float>()
    .mul<Vec2, Vec2>()
    .add<Vec2, Vec2>()
    .sub<Vec2, Vec2>()
    .tostring();

  slub::package lost_math = slub::package(L, "lost").package("math");
  lost_math.function("len", (float(*)(const Vec2&))&len);
  lost_math.function("squlen", (float(*)(const Vec2&))&squlen);
  lost_math.function("normalise", (Vec2&(*)(Vec2&))&normalise);
  lost_math.function("perpendicular", (Vec2(*)(Vec2&))&perpendicular);
  lost_math.function("angle", (float(*)(const Vec2&, const Vec2&))&angle);
  lost_math.function("compare", (bool(*)(const Vec2&, const Vec2&, float))&compare);

  if (luaL_dostring(L,
                "local v2 = lost.math.Vec2() "
                "local vec2 = lost.math.Vec2(10, 10) "
                "local vec2_2 = lost.math.Vec2(vec2) "
                "vec2_2.x = 11 "
                "vec2_2.y = 11 "
                "print(vec2 * vec2_2) "

// TODO: operator overloading
//                "print(vec2 * 10) "

                "print(vec2 + vec2_2) "
                "print(vec2 - vec2_2) "
                "local v = vec2 - vec2_2 "
                "print(v.x, v.y) "
                "print(lost.math.len(v)) "
                "print(lost.math.squlen(v)) "
                "print(tostring(lost.math.normalise(v))) "
                "print(tostring(lost.math.perpendicular(v))) "
                "print(lost.math.angle(v, v2)) "
                "print(lost.math.compare(v, v2, 0.1)) "
                "print(lost.math.compare(v, v, 0.1)) ")) {
    std::cout << lua_tostring(L, -1) << std::endl;
  }

  // TODO: simple functions

  slub::function(L, "testing0", &testing0);
  slub::function(L, "testing1", &testing1);
  slub::function(L, "testing2", &testing2);
  slub::function(L, "testing3", &testing3);
  slub::function(L, "testing", &testing);

  if (luaL_dostring(L,
                    "testing0() "
                    "testing1(1) "
                    "testing2(1, 2) "
                    "testing3(1, 2, 3) "
                    "print(testing(1, 2, 3)) ")) {
    std::cout << lua_tostring(L, -1) << std::endl;
  }

  lua_gc(L, LUA_GCCOLLECT, 0);
  lua_close(L);

  return 0;
}
