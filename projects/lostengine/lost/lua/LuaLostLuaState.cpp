#include "lost/lua/LuaBindings.h"
#include "lost/lua/State.h"

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
      .def_readwrite("callstackSize", &State::callstackSize)
    ]
  ];
}
LOST_LUA_EXPORT_END
