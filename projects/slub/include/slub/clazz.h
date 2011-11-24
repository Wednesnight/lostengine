#ifndef SLUB_CLAZZ_H
#define SLUB_CLAZZ_H

#include "forward.h"
#include "constructor.h"
#include "exception.h"
#include "field.h"
#include "function.h"
#include "method.h"
#include "operators.h"
#include "registry.h"
#include "slub_lua.h"
#include "wrapper.h"

#include <string>

namespace slub {

  struct deleter {
    template<typename T>
    static void delete_(T* t) {
      delete t;
    }
  };

  struct null_deleter {
    template<typename T>
    static void delete_(T* t) {
    }
  };

  struct abstract_clazz {
    static int __index(lua_State* L);
    static int __index(lua_State* L, const std::string& className, bool fallback);
    
    static int __newindex(lua_State* L);
    
    static int __callMethod(lua_State* L);
    static int __callOperator(lua_State* L);
  };

  template<typename T, typename D = deleter>
  struct clazz : public abstract_clazz {

    lua_State* state;
    std::string name;
    registry* reg;

    static T* clazz_cast(const reference& ref) {
      return ref.cast<T*>();
    }

    clazz(lua_State* L, const std::string& name, const std::string& prefix = "", int target = -1)
    : state(L), name(prefix.size() > 0 ? prefix +"."+ name : name)
    {

      reg = registry::registerType<T>(this->name);

      lua_newtable(state);
      int methods = lua_gettop(state);
      
      luaL_newmetatable(state, this->name.c_str());
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
      lua_pushcfunction(state, __call);
      lua_settable(state, mt);            // mt.__call = ctor
      lua_setmetatable(state, methods);
      
      lua_pushliteral(state, "__gc");
      lua_pushcfunction(state, __gc);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__index");
      lua_pushlightuserdata(state, reg);
      lua_pushcclosure(state, __index, 1);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__newindex");
      lua_pushlightuserdata(state, reg);
      lua_pushcclosure(state, __newindex, 1);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__eq");
      lua_pushlightuserdata(state, reg);
      lua_pushstring(state, "__eq");
      lua_pushcclosure(state, __callOperator, 2);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__lt");
      lua_pushlightuserdata(state, reg);
      lua_pushstring(state, "__lt");
      lua_pushcclosure(state, __callOperator, 2);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__le");
      lua_pushlightuserdata(state, reg);
      lua_pushstring(state, "__le");
      lua_pushcclosure(state, __callOperator, 2);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__tostring");
      lua_pushlightuserdata(state, reg);
      lua_pushstring(state, "__tostring");
      lua_pushcclosure(state, __callOperator, 2);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__add");
      lua_pushlightuserdata(state, reg);
      lua_pushstring(state, "__add");
      lua_pushcclosure(state, __callOperator, 2);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__sub");
      lua_pushlightuserdata(state, reg);
      lua_pushstring(state, "__sub");
      lua_pushcclosure(state, __callOperator, 2);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__mul");
      lua_pushlightuserdata(state, reg);
      lua_pushstring(state, "__mul");
      lua_pushcclosure(state, __callOperator, 2);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__div");
      lua_pushlightuserdata(state, reg);
      lua_pushstring(state, "__div");
      lua_pushcclosure(state, __callOperator, 2);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__mod");
      lua_pushlightuserdata(state, reg);
      lua_pushstring(state, "__mod");
      lua_pushcclosure(state, __callOperator, 2);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__pow");
      lua_pushlightuserdata(state, reg);
      lua_pushstring(state, "__pow");
      lua_pushcclosure(state, __callOperator, 2);
      lua_settable(state, metatable);
      
      lua_pushliteral(state, "__unm");
      lua_pushlightuserdata(state, reg);
      lua_pushstring(state, "__unm");
      lua_pushcclosure(state, __callOperator, 2);
      lua_settable(state, metatable);
      
      lua_pop(state, 2);  // drop metatable and method table

      function("cast", clazz_cast);
    }

    template<typename B>
    clazz& extends() {
      registry* base = registry::get(typeid(B));
      reg->registerBase(base);
      return *this;
    }

    clazz& constructor() {
      reg->addConstructor(new slub::constructor<T>());
      return *this;
    }
    
    template<typename arg1>
    clazz& constructor() {
      reg->addConstructor(new slub::constructor<T, arg1>());
      return *this;
    }
    
    template<typename arg1, typename arg2>
    clazz& constructor() {
      reg->addConstructor(new slub::constructor<T, arg1, arg2>());
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3>
    clazz& constructor() {
      reg->addConstructor(new slub::constructor<T, arg1, arg2, arg3>());
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3, typename arg4>
    clazz& constructor() {
      reg->addConstructor(new slub::constructor<T, arg1, arg2, arg3, arg4>());
      return *this;
    }
    
    template<typename F>
    clazz& field(const std::string& fieldName, F T::*m) {
      reg->addField(fieldName, new slub::field<T, F>(m));
      return *this;
    }

    template<typename F>
    clazz& field(const std::string& fieldName, F (T::*getter)(), void (T::*setter)(F)) {
      reg->addField(fieldName, new slub::field_method<T, F>(getter, setter));
      return *this;
    }
    
    template<typename F>
    clazz& field(const std::string& fieldName, F (T::*getter)() const, void (T::*setter)(F)) {
      reg->addField(fieldName, new slub::field_method_const<T, F>(getter, setter));
      return *this;
    }
    
    template<typename F>
    clazz& field(const std::string& fieldName, F (*getter)(T*), void (*setter)(T*, F)) {
      reg->addField(fieldName, new slub::field_function<T, F>(getter, setter));
      return *this;
    }
    
    template<typename ret>
    clazz& method(const std::string& methodName, ret (T::*m)()) {
      reg->addMethod(methodName, new slub::method<T, ret>(m));
      return *this;
    }
    
    clazz& method(const std::string& methodName, void (T::*m)()) {
      reg->addMethod(methodName, new slub::method<T>(m));
      return *this;
    }
    
    template<typename ret, typename arg1>
    clazz& method(const std::string& methodName, ret (T::*m)(arg1)) {
      reg->addMethod(methodName, new slub::method<T, ret, arg1>(m));
      return *this;
    }
    
    template<typename arg1>
    clazz& method(const std::string& methodName, void (T::*m)(arg1)) {
      reg->addMethod(methodName, new slub::method<T, void, arg1>(m));
      return *this;
    }
    
    template<typename ret, typename arg1, typename arg2>
    clazz& method(const std::string& methodName, ret (T::*m)(arg1, arg2)) {
      reg->addMethod(methodName, new slub::method<T, ret, arg1, arg2>(m));
      return *this;
    }
    
    template<typename arg1, typename arg2>
    clazz& method(const std::string& methodName, T* t, void (T::*m)(arg1, arg2)) {
      reg->addMethod(methodName, new slub::method<T, void, arg1, arg2>(m));
      return *this;
    }

    template<typename ret, typename arg1, typename arg2, typename arg3>
    clazz& method(const std::string& methodName, ret (T::*m)(arg1, arg2, arg3)) {
      reg->addMethod(methodName, new slub::method<T, ret, arg1, arg2, arg3>(m));
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3>
    clazz& method(const std::string& methodName, T* t, void (T::*m)(arg1, arg2, arg3)) {
      reg->addMethod(methodName, new slub::method<T, void, arg1, arg2, arg3>(m));
      return *this;
    }
    
    template<typename ret, typename arg1, typename arg2, typename arg3, typename arg4>
    clazz& method(const std::string& methodName, ret (T::*m)(arg1, arg2, arg3, arg4)) {
      reg->addMethod(methodName, new slub::method<T, ret, arg1, arg2, arg3, arg4>(m));
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3, typename arg4>
    clazz& method(const std::string& methodName, T* t, void (T::*m)(arg1, arg2, arg3, arg4)) {
      reg->addMethod(methodName, new slub::method<T, void, arg1, arg2, arg3, arg4>(m));
      return *this;
    }
    
    template<typename ret, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5>
    clazz& method(const std::string& methodName, ret (T::*m)(arg1, arg2, arg3, arg4, arg5)) {
      reg->addMethod(methodName, new slub::method<T, ret, arg1, arg2, arg3, arg4, arg5>(m));
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3, typename arg4, typename arg5>
    clazz& method(const std::string& methodName, T* t, void (T::*m)(arg1, arg2, arg3, arg4, arg5)) {
      reg->addMethod(methodName, new slub::method<T, void, arg1, arg2, arg3, arg4, arg5>(m));
      return *this;
    }
    
    template<typename ret, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6>
    clazz& method(const std::string& methodName, ret (T::*m)(arg1, arg2, arg3, arg4, arg5, arg6)) {
      reg->addMethod(methodName, new slub::method<T, ret, arg1, arg2, arg3, arg4, arg5, arg6>(m));
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6>
    clazz& method(const std::string& methodName, T* t, void (T::*m)(arg1, arg2, arg3, arg4, arg5, arg6)) {
      reg->addMethod(methodName, new slub::method<T, void, arg1, arg2, arg3, arg4, arg5, arg6>(m));
      return *this;
    }
    
    template<typename ret, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6, typename arg7>
    clazz& method(const std::string& methodName, ret (T::*m)(arg1, arg2, arg3, arg4, arg5, arg6, arg7)) {
      reg->addMethod(methodName, new slub::method<T, ret, arg1, arg2, arg3, arg4, arg5>(m));
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6, typename arg7>
    clazz& method(const std::string& methodName, T* t, void (T::*m)(arg1, arg2, arg3, arg4, arg5, arg6, arg7)) {
      reg->addMethod(methodName, new slub::method<T, void, arg1, arg2, arg3, arg4, arg5, arg6, arg7>(m));
      return *this;
    }
    
    template<typename ret>
    clazz& method(const std::string& methodName, ret (T::*m)() const) {
      reg->addMethod(methodName, new slub::const_method<T, ret>(m));
      return *this;
    }
    
    clazz& method(const std::string& methodName, void (T::*m)() const) {
      reg->addMethod(methodName, new slub::const_method<T>(m));
      return *this;
    }
    
    template<typename ret, typename arg1>
    clazz& method(const std::string& methodName, ret (T::*m)(arg1) const) {
      reg->addMethod(methodName, new slub::const_method<T, ret, arg1>(m));
      return *this;
    }
    
    template<typename arg1>
    clazz& method(const std::string& methodName, void (T::*m)(arg1) const) {
      reg->addMethod(methodName, new slub::const_method<T, void, arg1>(m));
      return *this;
    }
    
    template<typename ret, typename arg1, typename arg2>
    clazz& method(const std::string& methodName, ret (T::*m)(arg1, arg2) const) {
      reg->addMethod(methodName, new slub::const_method<T, ret, arg1, arg2>(m));
      return *this;
    }
    
    template<typename arg1, typename arg2>
    clazz& method(const std::string& methodName, T* t, void (T::*m)(arg1, arg2) const) {
      reg->addMethod(methodName, new slub::const_method<T, void, arg1, arg2>(m));
      return *this;
    }
    
    template<typename ret, typename arg1, typename arg2, typename arg3>
    clazz& method(const std::string& methodName, ret (T::*m)(arg1, arg2, arg3) const) {
      reg->addMethod(methodName, new slub::const_method<T, ret, arg1, arg2, arg3>(m));
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3, typename arg4>
    clazz& method(const std::string& methodName, T* t, void (T::*m)(arg1, arg2, arg3, arg4) const) {
      reg->addMethod(methodName, new slub::const_method<T, void, arg1, arg2, arg3, arg4>(m));
      return *this;
    }

    template<typename ret, typename arg1, typename arg2, typename arg3, typename arg4>
    clazz& method(const std::string& methodName, ret (T::*m)(arg1, arg2, arg3, arg4) const) {
      reg->addMethod(methodName, new slub::const_method<T, ret, arg1, arg2, arg3, arg4>(m));
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3>
    clazz& method(const std::string& methodName, T* t, void (T::*m)(arg1, arg2, arg3) const) {
      reg->addMethod(methodName, new slub::const_method<T, void, arg1, arg2, arg3>(m));
      return *this;
    }
    
    template<typename ret>
    clazz& method(const std::string& methodName, ret (*m)(T*)) {
      reg->addMethod(methodName, new slub::func_method<T, ret>(m));
      return *this;
    }
    
    clazz& method(const std::string& methodName, void (*m)(T*)) {
      reg->addMethod(methodName, new slub::func_method<T>(m));
      return *this;
    }
    
    template<typename ret, typename arg1>
    clazz& method(const std::string& methodName, ret (*m)(T*, arg1)) {
      reg->addMethod(methodName, new slub::func_method<T, ret, arg1>(m));
      return *this;
    }
    
    // TODO: add more lua_State* shortcuts
    template<typename ret>
    clazz& method(const std::string& methodName, ret (*m)(T*, lua_State*)) {
      reg->addMethod(methodName, new slub::func_method<T, ret, lua_State*>(m));
      return *this;
    }
    
    template<typename arg1>
    clazz& method(const std::string& methodName, void (*m)(T*, arg1)) {
      reg->addMethod(methodName, new slub::func_method<T, void, arg1>(m));
      return *this;
    }
    
    template<typename ret, typename arg1, typename arg2>
    clazz& method(const std::string& methodName, ret (*m)(T*, arg1, arg2)) {
      reg->addMethod(methodName, new slub::func_method<T, ret, arg1, arg2>(m));
      return *this;
    }
    
    template<typename arg1, typename arg2>
    clazz& method(const std::string& methodName, T* t, void (*m)(T*, arg1, arg2)) {
      reg->addMethod(methodName, new slub::func_method<T, void, arg1, arg2>(m));
      return *this;
    }
    
    template<typename ret, typename arg1, typename arg2, typename arg3>
    clazz& method(const std::string& methodName, ret (*m)(T*, arg1, arg2, arg3)) {
      reg->addMethod(methodName, new slub::func_method<T, ret, arg1, arg2, arg3>(m));
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3>
    clazz& method(const std::string& methodName, T* t, void (*m)(T*, arg1, arg2, arg3)) {
      reg->addMethod(methodName, new slub::func_method<T, void, arg1, arg2, arg3>(m));
      return *this;
    }

    template<typename ret, typename arg1, typename arg2, typename arg3, typename arg4>
    clazz& method(const std::string& methodName, ret (*m)(T*, arg1, arg2, arg3, arg4)) {
      reg->addMethod(methodName, new slub::func_method<T, ret, arg1, arg2, arg3, arg4>(m));
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3, typename arg4>
    clazz& method(const std::string& methodName, T* t, void (*m)(T*, arg1, arg2, arg3, arg4)) {
      reg->addMethod(methodName, new slub::func_method<T, void, arg1, arg2, arg3, arg4>(m));
      return *this;
    }
    
    template<typename ret, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5>
    clazz& method(const std::string& methodName, ret (*m)(T*, arg1, arg2, arg3, arg4, arg5)) {
      reg->addMethod(methodName, new slub::func_method<T, ret, arg1, arg2, arg3, arg4, arg5>(m));
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3, typename arg4, typename arg5>
    clazz& method(const std::string& methodName, T* t, void (*m)(T*, arg1, arg2, arg3, arg4, arg5)) {
      reg->addMethod(methodName, new slub::func_method<T, void, arg1, arg2, arg3, arg4, arg5>(m));
      return *this;
    }
    
    template<typename ret, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6>
    clazz& method(const std::string& methodName, ret (*m)(T*, arg1, arg2, arg3, arg4, arg5, arg6)) {
      reg->addMethod(methodName, new slub::func_method<T, ret, arg1, arg2, arg3, arg4, arg5, arg6>(m));
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6>
    clazz& method(const std::string& methodName, T* t, void (*m)(T*, arg1, arg2, arg3, arg4, arg5, arg6)) {
      reg->addMethod(methodName, new slub::func_method<T, void, arg1, arg2, arg3, arg4, arg5, arg6>(m));
      return *this;
    }
    
    template<typename ret, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6, typename arg7>
    clazz& method(const std::string& methodName, ret (*m)(T*, arg1, arg2, arg3, arg4, arg5, arg6, arg7)) {
      reg->addMethod(methodName, new slub::func_method<T, ret, arg1, arg2, arg3, arg4, arg5, arg6, arg7>(m));
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6, typename arg7>
    clazz& method(const std::string& methodName, T* t, void (*m)(T*, arg1, arg2, arg3, arg4, arg5, arg6, arg7)) {
      reg->addMethod(methodName, new slub::func_method<T, void, arg1, arg2, arg3, arg4, arg5, arg6, arg7>(m));
      return *this;
    }
    
    clazz& eq() {
      reg->addOperator("__eq", new eq_operator<T, T>());
      return *this;
    }

    template<typename F>
    clazz& eq() {
      reg->addOperator("__eq", new eq_operator<T, F>());
      return *this;
    }
    
    clazz& lt() {
      reg->addOperator("__lt", new lt_operator<T, T>());
      return *this;
    }
    
    template<typename F>
    clazz& lt() {
      reg->addOperator("__lt", new lt_operator<T, F>());
      return *this;
    }
    
    clazz& le() {
      reg->addOperator("__le", new le_operator<T, T>());
      return *this;
    }
    
    template<typename F>
    clazz& le() {
      reg->addOperator("__le", new le_operator<T, F>());
      return *this;
    }
    
    clazz& tostring() {
      reg->addOperator("__tostring", new tostring_operator<T>());
      return *this;
    }
    
    template<typename R, typename F>
    clazz& add() {
      reg->addOperator("__add", new add_operator<T, R, F>());
      return *this;
    }
    
    template<typename R, typename F>
    clazz& sub() {
      reg->addOperator("__sub", new sub_operator<T, R, F>());
      return *this;
    }
    
    template<typename R, typename F>
    clazz& mul() {
      reg->addOperator("__mul", new mul_operator<T, R, F>());
      return *this;
    }
    
    template<typename R, typename F>
    clazz& div() {
      reg->addOperator("__div", new div_operator<T, R, F>());
      return *this;
    }
    
    template<typename R, typename F>
    clazz& mod() {
      reg->addOperator("__mod", new mod_operator<T, R, F>());
      return *this;
    }
    
    template<typename R, typename F>
    clazz& pow() {
      reg->addOperator("__pow", new pow_operator<T, R, F>());
      return *this;
    }
    
    clazz& unm() {
      reg->addOperator("__unm", new unm_operator<T, T>());
      return *this;
    }

    template<typename R>
    clazz& unm() {
      reg->addOperator("__unm", new unm_operator<T, R>());
      return *this;
    }
    
    clazz& function(const std::string& name, void (*f)()) {
      luaL_getmetatable(state, this->name.c_str());
      lua_getfield(state, -1, "__metatable");
      slub::function(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 2);
      return *this;
    }
    
    template<typename arg1>
    clazz& function(const std::string& name, void (*f)(arg1)) {
      luaL_getmetatable(state, this->name.c_str());
      lua_getfield(state, -1, "__metatable");
      slub::function<arg1>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 2);
      return *this;
    }
    
    template<typename arg1, typename arg2>
    clazz& function(const std::string& name, void (*f)(arg1, arg2)) {
      luaL_getmetatable(state, this->name.c_str());
      lua_getfield(state, -1, "__metatable");
      slub::function<arg1, arg2>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 2);
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3>
    clazz& function(const std::string& name, void (*f)(arg1, arg2, arg3)) {
      luaL_getmetatable(state, this->name.c_str());
      lua_getfield(state, -1, "__metatable");
      slub::function<arg1, arg2, arg3>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 2);
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3, typename arg4>
    clazz& function(const std::string& name, void (*f)(arg1, arg2, arg3, arg4)) {
      luaL_getmetatable(state, this->name.c_str());
      lua_getfield(state, -1, "__metatable");
      slub::function<arg1, arg2, arg3, arg4>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 2);
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3, typename arg4, typename arg5>
    clazz& function(const std::string& name, void (*f)(arg1, arg2, arg3, arg4, arg5)) {
      luaL_getmetatable(state, this->name.c_str());
      lua_getfield(state, -1, "__metatable");
      slub::function<arg1, arg2, arg3, arg4, arg5>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 2);
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6>
    clazz& function(const std::string& name, void (*f)(arg1, arg2, arg3, arg4, arg5, arg6)) {
      luaL_getmetatable(state, this->name.c_str());
      lua_getfield(state, -1, "__metatable");
      slub::function<arg1, arg2, arg3, arg4, arg5, arg6>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 2);
      return *this;
    }
    
    template<typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6, typename arg7>
    clazz& function(const std::string& name, void (*f)(arg1, arg2, arg3, arg4, arg5, arg6, arg7)) {
      luaL_getmetatable(state, this->name.c_str());
      lua_getfield(state, -1, "__metatable");
      slub::function<arg1, arg2, arg3, arg4, arg5, arg6, arg7>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 2);
      return *this;
    }
    
    template<typename R>
    clazz& function(const std::string& name, R (*f)()) {
      luaL_getmetatable(state, this->name.c_str());
      lua_getfield(state, -1, "__metatable");
      slub::function<R>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 2);
      return *this;
    }
    
    template<typename R, typename arg1>
    clazz& function(const std::string& name, R (*f)(arg1)) {
      luaL_getmetatable(state, this->name.c_str());
      lua_getfield(state, -1, "__metatable");
      slub::function<R, arg1>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 2);
      return *this;
    }
    
    template<typename R, typename arg1, typename arg2>
    clazz& function(const std::string& name, R (*f)(arg1, arg2)) {
      luaL_getmetatable(state, this->name.c_str());
      lua_getfield(state, -1, "__metatable");
      slub::function<R, arg1, arg2>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 2);
      return *this;
    }
    
    template<typename R, typename arg1, typename arg2, typename arg3>
    clazz& function(const std::string& name, R (*f)(arg1, arg2, arg3)) {
      luaL_getmetatable(state, this->name.c_str());
      lua_getfield(state, -1, "__metatable");
      slub::function<R, arg1, arg2, arg3>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 2);
      return *this;
    }
    
    template<typename R, typename arg1, typename arg2, typename arg3, typename arg4>
    clazz& function(const std::string& name, R (*f)(arg1, arg2, arg3, arg4)) {
      luaL_getmetatable(state, this->name.c_str());
      lua_getfield(state, -1, "__metatable");
      slub::function<R, arg1, arg2, arg3, arg4>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 2);
      return *this;
    }
    
    template<typename R, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5>
    clazz& function(const std::string& name, R (*f)(arg1, arg2, arg3, arg4, arg5)) {
      luaL_getmetatable(state, this->name.c_str());
      lua_getfield(state, -1, "__metatable");
      slub::function<R, arg1, arg2, arg3, arg4, arg5>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 2);
      return *this;
    }
    
    template<typename R, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6>
    clazz& function(const std::string& name, R (*f)(arg1, arg2, arg3, arg4, arg5, arg6)) {
      luaL_getmetatable(state, this->name.c_str());
      lua_getfield(state, -1, "__metatable");
      slub::function<R, arg1, arg2, arg3, arg4, arg5, arg6>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 2);
      return *this;
    }
    
    template<typename R, typename arg1, typename arg2, typename arg3, typename arg4, typename arg5, typename arg6, typename arg7>
    clazz& function(const std::string& name, R (*f)(arg1, arg2, arg3, arg4, arg5, arg6, arg7)) {
      luaL_getmetatable(state, this->name.c_str());
      lua_getfield(state, -1, "__metatable");
      slub::function<R, arg1, arg2, arg3, arg4, arg5, arg6, arg7>(state, name, f, this->name, lua_gettop(state));
      lua_pop(state, 2);
      return *this;
    }
    
    template<typename C>
    clazz& enumerated(const std::string& constantName, const C& value) {
      return constant<int>(constantName, value);
    }
    
    template<typename C>
    clazz& constant(const std::string& constantName, const C& value) {
      luaL_getmetatable(state, name.c_str());
      lua_getfield(state, -1, "__metatable");
      converter<C>::push(state, value);
      lua_setfield(state, -2, constantName.c_str());
      lua_pop(state, 2);
      return *this;
    }
    
  private:

    static int __call(lua_State* L) {
      registry* r = registry::get(typeid(T));
      if (r->containsConstructor()) {
        T* instance = r->getConstructor(L)->newInstance<T>(L);
        wrapper<T*>* w = wrapper<T*>::create(L, typeid(T));
        w->ref = instance;
        w->gc = true;
        luaL_getmetatable(L, r->getTypeName().c_str());
        lua_setmetatable(L, -2);
        return 1;
      }
      return 0;
    }

    static int __gc(lua_State* L) {
      wrapper<T*>* w = static_cast<wrapper<T*>*>(luaL_checkudata(L, 1, registry::get(typeid(T))->getTypeName().c_str()));
      if (w->gc) {
        if (w->holder != NULL) {
          D::delete_(w->holder);
        }
        else {
          D::delete_(w->ref);
        }
      }
      else {
        w->ref = NULL;
      }
      return 0;
    }
    
  };

}

#endif
