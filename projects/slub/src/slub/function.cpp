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

#include "../../include/slub/config.h"
#include "../../include/slub/exception.h"
#include "../../include/slub/function.h"

#include <iostream>
#include <stdexcept>

namespace slub {

  function_holder function_holder::instance;
  
  function_holder::~function_holder() {
//    std::cout << "cleanup functions" << std::endl;
    
    for (map<string, list<abstract_function_wrapper*> >::iterator idx = functions.begin(); idx != functions.end(); ++idx) {
      for (list<abstract_function_wrapper*>::iterator fidx = idx->second.begin(); fidx != idx->second.end(); ++fidx) {
        delete *fidx;
      }
    }
    functions.clear();
  }

  void function_holder::add(lua_State* L, const string& name, abstract_function_wrapper* f, const string& prefix, int target) {
    string qualifiedName = prefix.size() > 0 ? prefix +"."+ name : name;

    instance.functions[qualifiedName].push_back(f);

    lua_pushstring(L, qualifiedName.c_str());
    lua_pushcclosure(L, call, 1);
    lua_setfield(L, target != -1 ? target : LUA_GLOBALSINDEX, name.c_str());
  }

  int function_holder::call(lua_State* L) {
    string name(lua_tolstring(L, lua_upvalueindex(1), NULL));
    for (list<abstract_function_wrapper*>::iterator idx = instance.functions[name].begin(); idx != instance.functions[name].end(); ++idx) {
      abstract_function_wrapper* f = *idx;
      if (f->check(L)) {
        int num = lua_gettop(L);
        f->call(L);
        return lua_gettop(L) - num;
      }
    }
    OverloadNotFoundException e(name);
    lua_pushstring(L, e.what());
    lua_error(L);
    throw e;
  }

}
