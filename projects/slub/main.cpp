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

#include <iostream>
#include <iosfwd>

#include <slub/slub.h>
#include <slub/globals.h>

#include "foo.h"

using namespace slub;

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

enum enm {
  e1,
  e2
};

int main (int argc, char * const argv[]) {

  baz b(0,0);
  std::cout << typeid(b).name() << std::endl;
  std::cout << typeid(&b).name() << std::endl;
  std::cout << typeid(b.get()).name() << std::endl;
  std::cout << typeid(*b.get()).name() << std::endl;

  lua_State *L = lua_open();

  luaopen_base(L);
  luaopen_table(L);
  luaopen_string(L);
  luaopen_math(L);
  luaopen_debug(L);

  {
    globals _G = globals(L);
    std::cout << _G["math"].typeName() << std::endl;
    std::cout << _G["math"]["abs"].typeName() << std::endl;
    std::cout << _G["math"]["abs"].operator()<int, int>(-2) << std::endl;

    _G["foobarbaz"] = "argrml";
    std::cout << _G["foobarbaz"].cast<std::string>() << std::endl;
  }

  slub::package enum_ = slub::package(L, "enum");
  enum_.enumerated("e1", e1);
  enum_.enumerated("e2", e2);

  if (luaL_dostring(L, "print(enum.e1) print(enum.e2) ")) {
    std::cout << lua_tostring(L, -1) << std::endl;
  }
  
  slub::clazz<foo>(L, "foo")
    .constructor<int>()
    .constructor<int, int>()
    .field("bar", &foo::bar)
    .method("doStuff", (void(foo::*)(void))&foo::doStuff)
    .method("doStuff", (void(foo::*)(int))&foo::doStuff)
    .method("getFoo", &foo::getFoo)
    .eq()
    .lt()
    .le()
    .tostring()
    .add<int, int>()
    .sub<int, int>()
    .mul<int, int>()
    .div<int, int>()
    .mod<int, int>()
    .pow<int, int>()
    .unm<int>();

  if (luaL_dofile(L, "foo.lua")) {
    std::cout << lua_tostring(L, -1) << std::endl;
  }

  slub::clazz<baz>(L, "baz").extends<foo>()
    .constructor<int, int>()
    .method("get", &baz::get)
    .method("print", &baz::print);

  if (luaL_dostring(L,
                    "local b = baz(10, 11) "
                    "print(tostring(b)) "
                    "print(b.bar) "
                    "b:doStuff() "
                    "b:doStuff(100) "
                    "local f = foo(10) "
                    "print(f:getFoo(b)) "
                    "b:print(b:get()) ")) {
    std::cout << lua_tostring(L, -1) << std::endl;
  }

  slub::clazz<std::string>(L, "str").constructor();

  if (luaL_dostring(L, "local s = str()")) {
    std::cout << lua_tostring(L, -1) << std::endl;
  }

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
