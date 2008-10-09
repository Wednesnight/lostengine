#include <boost/shared_ptr.hpp>
#include "lost/lua/State.h"
#include "lost/application/Timer.h"

#include "lost/lua/bindings/LostApplicationTimer.h"

using namespace luabind;
using namespace lost::application;
using namespace lost::event;

namespace lost
{
  namespace lua
  {
    void LostApplicationTimer(lost::lua::State& state)
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
