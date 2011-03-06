#include "lost/lua/bindings/LostProfiler.h"
#include "lost/lua/lua.h"

#include "lost/profiler/Blackbox.h"

using namespace luabind;

namespace lost
{
  namespace lua
  {

    void bb_inc(const std::string& name) { BB_INC(name); }
    void bb_dec(const std::string& name) { BB_DEC(name); }
    void bb_set_clear(const std::string& name, bool flag) { BB_SET_CLEAR(name,flag); }
    void bb_set(const std::string& name,float val) { BB_SET(name,val); }

    void LostProfilerBlackbox(lua_State* state)
    {
      module(state,"lost")
      [
        namespace_("profiler")
        [
          def("bb_inc",&bb_inc),
          def("bb_dec",&bb_dec),
          def("bb_set_clear",&bb_set_clear),
          def("bb_set",&bb_set)
        ]
      ];
    }

    void LostProfilerBBCount(lua_State* state)
    {
      module(state,"lost")
      [
        namespace_("profiler")
        [
          class_<lost::profiler::BBCount>("BBCount")
          .def(constructor<const std::string&>())
        ]
      ];
    }
    
    void LostProfiler(lua_State* state)
    {
      LostProfilerBlackbox(state);
      LostProfilerBBCount(state);
    }

  }
}