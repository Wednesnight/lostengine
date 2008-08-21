#include "lost/lua/Luabindings.h"
#include "lost/event/Event.h"

using namespace luabind;
using namespace lost::event;

LOST_LUA_EXPORT_BEGIN(LuaLostEventEvent)
{
  module(state, "lost")
  [
    namespace_("event")
    [
      class_<Event, boost::shared_ptr<Event> >("Event")
       .def(constructor<std::string>()) 
       .def_readwrite("type", &Event::type)
    ]
  ];
}
LOST_LUA_EXPORT_END
