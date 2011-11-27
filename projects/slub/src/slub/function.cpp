#include "../../include/slub/config.h"
#include "../../include/slub/function.h"

#include <iostream>
#include <stdexcept>

namespace slub {

  function_holder function_holder::instance;
  
  function_holder::~function_holder() {
//    std::cout << "cleanup functions" << std::endl;
    
    for (std::map<string, std::list<abstract_function_wrapper*> >::iterator idx = functions.begin(); idx != functions.end(); ++idx) {
      for (std::list<abstract_function_wrapper*>::iterator fidx = idx->second.begin(); fidx != idx->second.end(); ++fidx) {
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
    for (std::list<abstract_function_wrapper*>::iterator idx = instance.functions[name].begin(); idx != instance.functions[name].end(); ++idx) {
      abstract_function_wrapper* f = *idx;
      if (f->check(L)) {
        int num = lua_gettop(L);
        f->call(L);
        return lua_gettop(L) - num;
      }
    }
    throw std::runtime_error("No matching overload found, candidates: ...");
  }

}