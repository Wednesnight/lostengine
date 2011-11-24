#include "lost/lua/bindings/LostProfiler.h"
#include "lost/lua/lua.h"
#include "lost/profiler/Blackbox.h"

#include <slub/slub.h>

namespace lost
{
  namespace lua
  {

    void bb_inc(const string& name) { BB_INC(name); }
    void bb_dec(const string& name) { BB_DEC(name); }
    void bb_set_clear(const string& name, bool flag) { BB_SET_CLEAR(name,flag); }
    void bb_set(const string& name,float val) { BB_SET(name,val); }

    void LostProfilerBlackbox(lua_State* state)
    {
      slub::package(state,"lost").package("profiler")
        .function("bb_inc",&bb_inc)
        .function("bb_dec",&bb_dec)
        .function("bb_set_clear",&bb_set_clear)
        .function("bb_set",&bb_set);
    }

    void LostProfilerBBCount(lua_State* state)
    {
      slub::package(state,"lost").package("profiler")
        .clazz<lost::profiler::BBCount>("BBCount")
          .constructor<const string&>();
    }
    
    void LostProfiler(lua_State* state)
    {
      LostProfilerBlackbox(state);
      LostProfilerBBCount(state);
    }

  }
}