#ifndef SLUB_CLAZZ_H
#define SLUB_CLAZZ_H

#include <slub/converter.h>
#include <slub/field.h>
#include <slub/method.h>
#include <slub/registry.h>
#include <slub/wrapper.h>
#include <string>
#include <sstream>
#include <map>

namespace slub {

  struct fields {
    
    static fields instance;
    
    std::map<std::string, abstract_field*> fieldMap;
    std::map<std::string, abstract_method*> methodMap;
    
    ~fields() {
      std::cout << "cleanup fields" << std::endl;

      for (std::map<std::string, abstract_field*>::iterator idx = fieldMap.begin(); idx != fieldMap.end(); ++idx) {
        delete idx->second;
      }
      fieldMap.clear();
      for (std::map<std::string, abstract_method*>::iterator idx = methodMap.begin(); idx != methodMap.end(); ++idx) {
        delete idx->second;
      }
      methodMap.clear();
    }
    
    static void addField(const std::string& fieldName, abstract_field* field) {
      instance.fieldMap[fieldName] = field;
    }
    
    static bool containsField(const std::string& fieldName) {
      return instance.fieldMap.find(fieldName) != instance.fieldMap.end();
    }
    
    static abstract_field* getField(const std::string& fieldName) {
      return instance.fieldMap[fieldName];
    }
    
    static void addMethod(const std::string& methodName, abstract_method* method) {
      instance.methodMap[methodName] = method;
    }
    
    static bool containsMethod(const std::string& methodName) {
      return instance.methodMap.find(methodName) != instance.methodMap.end();
    }
    
    static abstract_method* getMethod(const std::string& methodName) {
      return instance.methodMap[methodName];
    }
    
  };

  fields fields::instance;

  template<typename T>
  struct constructor {

    static T* newInstance(lua_State* L) {
      return new T();
    }

  };

  template<typename T>
  struct clazz {

    lua_State* state;
    std::string name;

    clazz(const std::string& name, lua_State* L) : state(L), name(name) {
      registry<T*>::registerType(name);

      lua_newtable(L);
      int methods = lua_gettop(L);
      
      luaL_newmetatable(L, name.c_str());
      int metatable = lua_gettop(L);
      
      // store method table in globals so that
      // scripts can add functions written in Lua.
      lua_pushstring(L, name.c_str());
      lua_pushvalue(L, methods);
      lua_settable(L, LUA_GLOBALSINDEX);
      
      lua_pushliteral(L, "__metatable");
      lua_pushvalue(L, methods);
      lua_settable(L, metatable);  // hide metatable from Lua getmetatable()
      
      lua_newtable(L);                // mt for method table
      int mt = lua_gettop(L);
      lua_pushliteral(L, "__call");
      lua_pushstring(L, name.c_str());
      lua_pushcclosure(L, __call, 1);
      lua_settable(L, mt);            // mt.__call = ctor
      lua_setmetatable(L, methods);
      
      lua_pushliteral(L, "__gc");
      lua_pushstring(L, name.c_str());
      lua_pushcclosure(L, __gc, 1);
      lua_settable(L, metatable);
      
      lua_pushliteral(L, "__index");
      lua_pushstring(L, name.c_str());
      lua_pushcclosure(L, __index, 1);
      lua_settable(L, metatable);
      
      lua_pushliteral(L, "__newindex");
      lua_pushstring(L, name.c_str());
      lua_pushcclosure(L, __newindex, 1);
      lua_settable(L, metatable);
      
      lua_pushliteral(L, "__eq");
      lua_pushstring(L, name.c_str());
      lua_pushcclosure(L, __eq, 1);
      lua_settable(L, metatable);
      
      lua_pop(L, 2);  // drop metatable and method table
    }
    
    template<typename F>
    clazz& field(const std::string& fieldName, F T::*m) {
      fields::addField(fieldName, new slub::field<T, F>(m));
      return *this;
    }

    template<typename ret>
    clazz& method(const std::string& methodName, ret (T::*m)()) {
      fields::addMethod(methodName, new slub::method<T, ret>(m));
      return *this;
    }
    
    clazz& method(const std::string& methodName, void (T::*m)()) {
      fields::addMethod(methodName, new slub::method<T>(m));
      return *this;
    }
    
    template<typename ret, typename arg1>
    clazz& method(const std::string& methodName, ret (T::*m)(arg1)) {
      fields::addMethod(methodName, new slub::method<T, ret, arg1>(m));
      return *this;
    }
    
    template<typename arg1>
    clazz& method(const std::string& methodName, void (T::*m)(arg1)) {
      fields::addMethod(methodName, new slub::method<T, void, arg1>(m));
      return *this;
    }
    
    template<typename ret, typename arg1, typename arg2>
    clazz& method(const std::string& methodName, ret (T::*m)(arg1, arg2)) {
      fields::addMethod(methodName, new slub::method<T, ret, arg1, arg2>(m));
      return *this;
    }
    
    template<typename arg1, typename arg2>
    clazz& method(const std::string& methodName, T* t, void (T::*m)(arg1, arg2)) {
      fields::addMethod(methodName, new slub::method<T, void, arg1, arg2>(m));
      return *this;
    }

    clazz& tostring() {
      luaL_getmetatable(state, name.c_str());
      int metatable = lua_gettop(state);
      lua_pushliteral(state, "__tostring");
      lua_pushstring(state, name.c_str());
      lua_pushcclosure(state, __tostring, 1);
      lua_settable(state, metatable);
      return *this;
    }
    
    template<typename R, typename F>
    clazz& add() {
      luaL_getmetatable(state, name.c_str());
      int metatable = lua_gettop(state);
      lua_pushliteral(state, "__add");
      lua_pushstring(state, name.c_str());
      lua_pushcclosure(state, __add<R, F>, 1);
      lua_settable(state, metatable);
      return *this;
    }
    
    template<typename R, typename F>
    clazz& sub() {
      luaL_getmetatable(state, name.c_str());
      int metatable = lua_gettop(state);
      lua_pushliteral(state, "__sub");
      lua_pushstring(state, name.c_str());
      lua_pushcclosure(state, __sub<R, F>, 1);
      lua_settable(state, metatable);
      return *this;
    }
    
    template<typename R, typename F>
    clazz& mul() {
      luaL_getmetatable(state, name.c_str());
      int metatable = lua_gettop(state);
      lua_pushliteral(state, "__mul");
      lua_pushstring(state, name.c_str());
      lua_pushcclosure(state, __mul<R, F>, 1);
      lua_settable(state, metatable);
      return *this;
    }
    
    template<typename R, typename F>
    clazz& div() {
      luaL_getmetatable(state, name.c_str());
      int metatable = lua_gettop(state);
      lua_pushliteral(state, "__div");
      lua_pushstring(state, name.c_str());
      lua_pushcclosure(state, __div<R, F>, 1);
      lua_settable(state, metatable);
      return *this;
    }
    
    template<typename R, typename F>
    clazz& mod() {
      luaL_getmetatable(state, name.c_str());
      int metatable = lua_gettop(state);
      lua_pushliteral(state, "__mod");
      lua_pushstring(state, name.c_str());
      lua_pushcclosure(state, __mod<R, F>, 1);
      lua_settable(state, metatable);
      return *this;
    }
    
    template<typename R, typename F>
    clazz& pow() {
      luaL_getmetatable(state, name.c_str());
      int metatable = lua_gettop(state);
      lua_pushliteral(state, "__pow");
      lua_pushstring(state, name.c_str());
      lua_pushcclosure(state, __pow<R, F>, 1);
      lua_settable(state, metatable);
      return *this;
    }
    
    template<typename R>
    clazz& unm() {
      luaL_getmetatable(state, name.c_str());
      int metatable = lua_gettop(state);
      lua_pushliteral(state, "__unm");
      lua_pushstring(state, name.c_str());
      lua_pushcclosure(state, __unm<R>, 1);
      lua_settable(state, metatable);
      return *this;
    }
    
    clazz& lt() {
      luaL_getmetatable(state, name.c_str());
      int metatable = lua_gettop(state);
      lua_pushliteral(state, "__lt");
      lua_pushstring(state, name.c_str());
      lua_pushcclosure(state, __lt, 1);
      lua_settable(state, metatable);
      return *this;
    }
    
    clazz& le() {
      luaL_getmetatable(state, name.c_str());
      int metatable = lua_gettop(state);
      lua_pushliteral(state, "__le");
      lua_pushstring(state, name.c_str());
      lua_pushcclosure(state, __le, 1);
      lua_settable(state, metatable);
      return *this;
    }

  private:

    static int __call(lua_State* L) {
      T* instance = constructor<T>::newInstance(L);
      wrapper<T*>* w = (wrapper<T*>*) lua_newuserdata(L, sizeof(wrapper<T*>));
      w->ref = instance;
      w->gc = true;
      luaL_getmetatable(L, lua_tostring(L, lua_upvalueindex(1)));
      lua_setmetatable(L, -2);
      return 1;
    }
    
    static int __gc(lua_State* L) {
      wrapper<T*>* w = static_cast<wrapper<T*>*>(luaL_checkudata(L, 1, lua_tostring(L, lua_upvalueindex(1))));
      if (w->gc) {
        delete w->ref;
      }
      else {
        w->ref = NULL;
      }
      return 0;
    }
    
    static int __index(lua_State* L) {
      std::string name(lua_tostring(L, -1));
      if (fields::containsField(name)) {
        return fields::getField(name)->get(L);
      }
      else if (fields::containsMethod(name)) {
        return fields::getMethod(name)->call(L);
      }
      else {
        lua_pushstring(L, lua_tostring(L, lua_upvalueindex(1)));
        lua_gettable(L, LUA_GLOBALSINDEX);
        int methods = lua_gettop(L);
        
        lua_pushvalue(L, -2);
        lua_gettable(L, methods);
        
        return 1;
      }
    }
    
    static int __newindex(lua_State* L) {
      std::string name(lua_tostring(L, -2));
      if (fields::containsField(name)) {
        return fields::getField(name)->set(L);
      }
      else {
        lua_pushstring(L, lua_tostring(L, lua_upvalueindex(1)));
        lua_gettable(L, LUA_GLOBALSINDEX);
        int methods = lua_gettop(L);
        
        lua_pushvalue(L, -2);
        lua_setfield(L, methods, name.c_str());
        
        lua_pop(L, 1);
        return 0;
      }
    }
    
    static int __eq(lua_State* L) {
      wrapper<T*>* r = static_cast<wrapper<T*>*>(luaL_checkudata(L, 1, lua_tostring(L, lua_upvalueindex(1))));
      wrapper<T*>* r2 = static_cast<wrapper<T*>*>(luaL_checkudata(L, -1, lua_tostring(L, lua_upvalueindex(1))));
      lua_pushboolean(L, *r->ref == *r2->ref);
      return 1;
    }
    
    static int __lt(lua_State* L) {
      wrapper<T*>* r = static_cast<wrapper<T*>*>(luaL_checkudata(L, 1, lua_tostring(L, lua_upvalueindex(1))));
      wrapper<T*>* r2 = static_cast<wrapper<T*>*>(luaL_checkudata(L, -1, lua_tostring(L, lua_upvalueindex(1))));
      lua_pushboolean(L, *r->ref < *r2->ref);
      return 1;
    }
    
    static int __le(lua_State* L) {
      wrapper<T*>* r = static_cast<wrapper<T*>*>(luaL_checkudata(L, 1, lua_tostring(L, lua_upvalueindex(1))));
      wrapper<T*>* r2 = static_cast<wrapper<T*>*>(luaL_checkudata(L, -1, lua_tostring(L, lua_upvalueindex(1))));
      lua_pushboolean(L, *r->ref <= *r2->ref);
      return 1;
    }
    
    static int __tostring(lua_State* L) {
      wrapper<T*>* r = static_cast<wrapper<T*>*>(luaL_checkudata(L, 1, lua_tostring(L, lua_upvalueindex(1))));
      std::stringstream s;
      r->ref->operator<<(s);
      lua_pushstring(L, s.str().c_str());
      return 1;
    }
    
    template<typename R, typename F>
    static int __add(lua_State* L) {
      wrapper<T*>* r = static_cast<wrapper<T*>*>(luaL_checkudata(L, 1, lua_tostring(L, lua_upvalueindex(1))));
      converter<R>::push(L, *r->ref + converter<F>::get(L, -1));
      return 1;
    }
    
    template<typename R, typename F>
    static int __sub(lua_State* L) {
      wrapper<T*>* r = static_cast<wrapper<T*>*>(luaL_checkudata(L, 1, lua_tostring(L, lua_upvalueindex(1))));
      converter<R>::push(L, *r->ref - converter<F>::get(L, -1));
      return 1;
    }
    
    template<typename R, typename F>
    static int __mul(lua_State* L) {
      wrapper<T*>* r = static_cast<wrapper<T*>*>(luaL_checkudata(L, 1, lua_tostring(L, lua_upvalueindex(1))));
      converter<R>::push(L, *r->ref * converter<F>::get(L, -1));
      return 1;
    }
    
    template<typename R, typename F>
    static int __div(lua_State* L) {
      wrapper<T*>* r = static_cast<wrapper<T*>*>(luaL_checkudata(L, 1, lua_tostring(L, lua_upvalueindex(1))));
      converter<R>::push(L, *r->ref / converter<F>::get(L, -1));
      return 1;
    }
    
    template<typename R, typename F>
    static int __mod(lua_State* L) {
      wrapper<T*>* r = static_cast<wrapper<T*>*>(luaL_checkudata(L, 1, lua_tostring(L, lua_upvalueindex(1))));
      converter<R>::push(L, *r->ref % converter<F>::get(L, -1));
      return 1;
    }
    
    template<typename R, typename F>
    static int __pow(lua_State* L) {
      wrapper<T*>* r = static_cast<wrapper<T*>*>(luaL_checkudata(L, 1, lua_tostring(L, lua_upvalueindex(1))));
      converter<R>::push(L, *r->ref ^ converter<F>::get(L, -1));
      return 1;
    }
    
    template<typename R>
    static int __unm(lua_State* L) {
      wrapper<T*>* r = static_cast<wrapper<T*>*>(luaL_checkudata(L, 1, lua_tostring(L, lua_upvalueindex(1))));
      converter<R>::push(L, -(*r->ref));
      return 1;
    }
    
  };

}

#endif
