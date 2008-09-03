#include "lost/lua/bindings/LostApplicationTimerEvent.h"
#include "lost/application/TimerEvent.h"
#include "lost/lua/EventCast.h"

using namespace luabind;
using namespace lost::event;
using namespace lost::application;

LOST_LUA_EXPORT_BEGIN(LostApplicationTimerEvent)
{
  module(state, "lost")
  [
    namespace_("application")
    [
      class_<TimerEvent, boost::shared_ptr<TimerEvent>, boost::shared_ptr<Event> >("TimerEvent")
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
LOST_LUA_EXPORT_END
