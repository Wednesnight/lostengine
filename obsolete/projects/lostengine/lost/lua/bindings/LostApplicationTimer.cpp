#include "lost/lua/lua.h"
#include "lost/application/Timer.h"

#include "lost/lua/bindings/LostApplicationTimer.h"

using namespace luabind;
using namespace lost::application;
using namespace lost::event;

namespace lost
{
  namespace lua
  {
    void LostApplicationTimer(lua_State* state)
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
  }
}
