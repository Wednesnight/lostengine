#include "lost/lua/LuaBindings.h"
#include "lost/lua/State.h"
#include "lost/resource/File.h"

using namespace luabind;
using namespace lost::lua;

LOST_LUA_EXPORT_BEGIN(LuaLostLuaState)
{
  module(state, "lost")
  [
    namespace_("lua")
    [
      class_<State, boost::shared_ptr<State> >("State")
      .def(constructor<>())
     .def("doFile", (void(State::*)(const boost::shared_ptr<lost::resource::File>&)) &State::doFile)
      .def_readwrite("callstackSize", &State::callstackSize)
    ]
  ];
}
LOST_LUA_EXPORT_END
