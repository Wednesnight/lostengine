#ifndef SLUB_CLAZZ_H
#define SLUB_CLAZZ_H

#include <slub/slub_lua.h>
#include <slub/constructor.h>
#include <slub/converter.h>
#include <slub/field.h>
#include <slub/method.h>
#include <slub/operators.h>
#include <slub/registry.h>
#include <slub/wrapper.h>
#include <string>
#include <sstream>
#include <map>
#include <list>
#include <stdexcept>

namespace slub {

  struct fields {
    
    static fields instance;
    
    std::map<std::string, std::list<abstract_constructor*> > constructorMap;
    std::map<std::string, abstract_field*> fieldMap;
    std::map<std::string, std::list<abstract_method*> > methodMap;
    std::map<std::string, std::list<abstract_operator*> > operatorMap;
    
    ~fields();
    
    static void addConstructor(const std::string& className, abstract_constructor* ctor);
    static bool containsConstructor(const std::string& className);
    static abstract_constructor* getConstructor(const std::string& className, lua_State* L);
    
    static void addField(const std::string& fieldName, abstract_field* field);
    static bool containsField(const std::string& fieldName);
    static abstract_field* getField(const std::string& fieldName);
    
    static void addMethod(const std::string& methodName, abstract_method* method);
    static bool containsMethod(const std::string& methodName);
    static abstract_method* getMethod(const std::string& methodName, lua_State* L);
    
    static void addOperator(const std::string& operatorName, abstract_operator* op);
    static bool containsOperator(const std::string& operatorName);
    static abstract_operator* getOperator(const std::string& operatorName, lua_State* L);
    
  };

  template<typename T>
  struct clazz {

    lua_State* state;
    std::string name;
    int target;

    clazz(lua_State* L, const std::string& name, int target = -1) : state(L), name(name), target(target) {
      registry<T*>::registerType(name);

      lua_newtable(state);
      int methods = lua_gettop(state);
      
      luaL_newmetatable(state, name.c_str());
      int metatable = lua_gettop(state);
      
      // store method table in globals so that
      // scripts can add functions written in Lua.
      lua_pushvalue(state, methods);
      lua_setfield(state, target != -1 ? target : LUA_GLOBALSINDEX, name.c_str());
      
      lua_pushliteral(state, "__metatable");
      lua_pushvalue(state, methods);
      lua_settable(state, metatable);  // hide metatable from Lua getmetatable()
      
      lua_newtable(state);                // mt for method table
      int mt = lua_gettop(state);
      lua_pushliteral(state, "__call");
      lua_pushstring(state, name.c_str());
      lua_pushcclosure(state, __call, 1);
      lua_settable(state, mt);            // mt.__call = ctor
      lua_setmetatable(state, methods);
      
      lua_pushliteral(state, "__gc");
      lua_pushstring(state, name.c_str());
      lua_pushcclosure(state, __gc, 1);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__index");
      lua_pushstring(state, name.c_str());
      lua_pushcclosure(state, __index, 1);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__newindex");
      lua_pushstring(state, name.c_str());
      lua_pushcclosure(state, __newindex, 1);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__eq");
      lua_pushstring(state, (name + "__eq").c_str());
      lua_pushcclosure(state, __callOperator, 1);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__lt");
      lua_pushstring(state, (name + "__lt").c_str());
      lua_pushcclosure(state, __callOperator, 1);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__le");
      lua_pushstring(state, (name + "__le").c_str());
      lua_pushcclosure(state, __callOperator, 1);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__tostring");
      lua_pushstring(state, (name + "__tostring").c_str());
      lua_pushcclosure(state, __callOperator, 1);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__add");
      lua_pushstring(state, (name + "__add").c_str());
      lua_pushcclosure(state, __callOperator, 1);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__sub");
      lua_pushstring(state, (name + "__sub").c_str());
      lua_pushcclosure(state, __callOperator, 1);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__mul");
      lua_pushstring(state, (name + "__mul").c_str());
      lua_pushcclosure(state, __callOperator, 1);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__div");
      lua_pushstring(state, (name + "__div").c_str());
      lua_pushcclosure(state, __callOperator, 1);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__mod");
      lua_pushstring(state, (name + "__mod").c_str());
      lua_pushcclosure(state, __callOperator, 1);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__pow");
      lua_pushstring(state, (name + "__pow").c_str());
      lua_pushcclosure(state, __callOperator, 1);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__unm");
      lua_pushstring(state, (name + "__unm").c_str());
      lua_pushcclosure(state, __callOperator, 1);
      lua_settable(state, metatable);
      
      lua_pop(state, 2);  // drop metatable and method table
    }

    clazz& constructor() {
      fields::addConstructor(name, new slub::constructor<T>());
      return *this;
    }
    
    template<typename arg1>
    clazz& constructor() {
      fields::addConstructor(name, new slub::constructor<T, arg1>());
      return *this;
    }
    
    template<typename arg1, typename arg2>
    clazz& constructor() {
      fields::addConstructor(name, new slub::constructor<T, arg1, arg2>());
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3>
    clazz& constructor() {
      fields::addConstructor(name, new slub::constructor<T, arg1, arg2, arg3>());
      return *this;
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

    template<typename ret>
    clazz& method(const std::string& methodName, ret (T::*m)() const) {
      fields::addMethod(methodName, new slub::const_method<T, ret>(m));
      return *this;
    }
    
    clazz& method(const std::string& methodName, void (T::*m)() const) {
      fields::addMethod(methodName, new slub::const_method<T>(m));
      return *this;
    }
    
    template<typename ret, typename arg1>
    clazz& method(const std::string& methodName, ret (T::*m)(arg1) const) {
      fields::addMethod(methodName, new slub::const_method<T, ret, arg1>(m));
      return *this;
    }
    
    template<typename arg1>
    clazz& method(const std::string& methodName, void (T::*m)(arg1) const) {
      fields::addMethod(methodName, new slub::const_method<T, void, arg1>(m));
      return *this;
    }
    
    template<typename ret, typename arg1, typename arg2>
    clazz& method(const std::string& methodName, ret (T::*m)(arg1, arg2) const) {
      fields::addMethod(methodName, new slub::const_method<T, ret, arg1, arg2>(m));
      return *this;
    }
    
    template<typename arg1, typename arg2>
    clazz& method(const std::string& methodName, T* t, void (T::*m)(arg1, arg2) const) {
      fields::addMethod(methodName, new slub::const_method<T, void, arg1, arg2>(m));
      return *this;
    }
    
    clazz& eq() {
      fields::addOperator(name + "__eq", new eq_operator<T, T>());
      return *this;
    }

    template<typename F>
    clazz& eq() {
      fields::addOperator(name + "__eq", new eq_operator<T, F>());
      return *this;
    }
    
    clazz& lt() {
      fields::addOperator(name + "__lt", new lt_operator<T, T>());
      return *this;
    }
    
    template<typename F>
    clazz& lt() {
      fields::addOperator(name + "__lt", new lt_operator<T, F>());
      return *this;
    }
    
    clazz& le() {
      fields::addOperator(name + "__le", new le_operator<T, T>());
      return *this;
    }
    
    template<typename F>
    clazz& le() {
      fields::addOperator(name + "__le", new le_operator<T, F>());
      return *this;
    }
    
    clazz& tostring() {
      fields::addOperator(name + "__tostring", new tostring_operator<T>());
      return *this;
    }
    
    template<typename R, typename F>
    clazz& add() {
      fields::addOperator(name + "__add", new add_operator<T, R, F>());
      return *this;
    }
    
    template<typename R, typename F>
    clazz& sub() {
      fields::addOperator(name + "__sub", new sub_operator<T, R, F>());
      return *this;
    }
    
    template<typename R, typename F>
    clazz& mul() {
      fields::addOperator(name + "__mul", new mul_operator<T, R, F>());
      return *this;
    }
    
    template<typename R, typename F>
    clazz& div() {
      fields::addOperator(name + "__div", new div_operator<T, R, F>());
      return *this;
    }
    
    template<typename R, typename F>
    clazz& mod() {
      fields::addOperator(name + "__mod", new mod_operator<T, R, F>());
      return *this;
    }
    
    template<typename R, typename F>
    clazz& pow() {
      fields::addOperator(name + "__pow", new pow_operator<T, R, F>());
      return *this;
    }
    
    clazz& unm() {
      fields::addOperator(name + "__unm", new unm_operator<T, T>());
      return *this;
    }

    template<typename R>
    clazz& unm() {
      fields::addOperator(name + "__unm", new unm_operator<T, R>());
      return *this;
    }
    
  private:

    static int __call(lua_State* L) {
      std::string name(lua_tostring(L, lua_upvalueindex(1)));
      if (fields::containsConstructor(name)) {
        T* instance = fields::getConstructor(name, L)->newInstance<T>(L);
        wrapper<T*>* w = (wrapper<T*>*) lua_newuserdata(L, sizeof(wrapper<T*>));
        w->ref = instance;
        w->gc = true;
        luaL_getmetatable(L, lua_tostring(L, lua_upvalueindex(1)));
        lua_setmetatable(L, -2);
        return 1;
      }
      return 0;
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
        lua_pushvalue(L, -1);
        lua_pushcclosure(L, __callMethod, 1);
        return 1;
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
    
    static int __callMethod(lua_State* L) {
      return fields::getMethod(lua_tostring(L, lua_upvalueindex(1)), L)->call(L);
    }
    
    static int __callOperator(lua_State* L) {
      return fields::getOperator(lua_tostring(L, lua_upvalueindex(1)), L)->op(L);
    }

  };

}

#endif
