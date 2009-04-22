#ifndef LOST_LUA_LUA_H
#define LOST_LUA_LUA_H

extern "C" {
  #include "lua.h"
  #include "lauxlib.h"
  #include "lualib.h"
}

// get_pointer and get_const_holder are defined for boost::shared_ptr, but not
// for std::tr1::shared_ptr - they're easily defined here, though.
#if defined(HAVE_TR1_SHARED_PTR)
namespace luabind {

template<class T>
T * get_pointer(::lost::shared_ptr<T> const & p)
{
  return p.get();
}

template <class T>
::lost::shared_ptr<T const> * get_const_holder(::lost::shared_ptr<T> *)
{
  return 0;
}

};
#endif

#include <luabind/luabind.hpp>
#include "lost/platform/Platform.h"

// from here: http://www.codeproject.com/KB/graphics/luabindLuaAndOgre3d.aspx
#define LOST_LUA_CONST_START( class, state ) { object g = globals(state); object table = g[#class];
#define LOST_LUA_CONST( class, name ) table[#name] = class::name;
#define LOST_LUA_CONST_END }

#undef LUA_NUMBER
#undef LUA_NUMBER_SCAN
#undef LUA_NUMBER_FMT
#if TARGET_OS_IPHONE
  #define LUA_NUMBER        float
  #define LUA_NUMBER_SCAN		"%f"
  #define LUA_NUMBER_FMT		"%g"
#else
  #define LUA_NUMBER        double
  #define LUA_NUMBER_SCAN		"%lf"
  #define LUA_NUMBER_FMT		"%.14g"
#endif

#endif
