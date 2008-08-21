#include "lost/lua/Luabindings.h"
#include "lost/application/TimerEvent.h"

using namespace luabind;
using namespace lost::event;
using namespace lost::application;

LOST_LUA_EXPORT_BEGIN(LuaLostApplicationTimerEvent)
{
  module(state, "lost")
  [
   namespace_("application")
   [
    class_<TimerEvent, boost::shared_ptr<TimerEvent>, boost::shared_ptr<Event> >("TimerEvent")
    .def(constructor<std::string>()) 
    .def_readwrite("passedSec", &TimerEvent::passedSec)
    ]
   ];
  globals(state)["lost"]["application"]["TimerEvent"]["TIMER_FIRED"] = TimerEvent::TIMER_FIRED();
}
LOST_LUA_EXPORT_END
