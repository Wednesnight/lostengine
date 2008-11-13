#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/application/TimerEvent.h"
#include "lost/lua/EventCast.h"

#include "lost/lua/bindings/LostApplicationTimerEvent.h"

using namespace luabind;
using namespace lost::event;
using namespace lost::application;

namespace lost
{
  namespace lua
  {
    void LostApplicationTimerEvent(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<TimerEvent, Event, boost::shared_ptr<TimerEvent> >("TimerEvent")
          .def(constructor<std::string>()) 
          .def_readwrite("passedSec", &TimerEvent::passedSec)
          .scope
          [
            def("cast", &lost::lua::cast<TimerEvent>)
          ]
        ]
      ];
      globals(state)["lost"]["application"]["TimerEvent"]["TIMER_FIRED"] = TimerEvent::TIMER_FIRED();
    }
  }
}
