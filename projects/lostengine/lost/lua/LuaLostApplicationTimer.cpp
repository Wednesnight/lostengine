#include "lost/lua/Luabindings.h"
#include "lost/application/Timer.h"

using namespace luabind;
using namespace lost::application;
using namespace lost::event;

LOST_LUA_EXPORT_BEGIN(LuaLostApplicationTimer)
{
  module(state, "lost")
  [
    namespace_("application")
    [
      class_<Timer, EventDispatcher>("Timer")
      .def(constructor<std::string, double>())
    ]
  ];
}
LOST_LUA_EXPORT_END
