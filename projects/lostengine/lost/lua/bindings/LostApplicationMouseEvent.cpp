#include "lost/lua/bindings/LostApplicationMouseEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/lua/EventCast.h"

using namespace luabind;
using namespace lost::event;
using namespace lost::application;

LOST_LUA_EXPORT_BEGIN(LostApplicationMouseEvent)
{
  module(state, "lost")
  [
    namespace_("application")
    [
      class_<MouseEvent, boost::shared_ptr<MouseEvent>, boost::shared_ptr<Event> >("MouseEvent")
      .def(constructor<std::string>()) 
      .def_readwrite("pos", &MouseEvent::pos)
      .def_readwrite("button", &MouseEvent::button)
      .def_readwrite("pressed", &MouseEvent::pressed)
      .scope
      [
        def("cast", &lost::lua::cast<MouseEvent>)
      ]
    ]
  ];
  globals(state)["lost"]["application"]["MouseEvent"]["MOUSE_UP"] = MouseEvent::MOUSE_UP();
  globals(state)["lost"]["application"]["MouseEvent"]["MOUSE_DOWN"] = MouseEvent::MOUSE_DOWN();
  globals(state)["lost"]["application"]["MouseEvent"]["MOUSE_MOVE"] = MouseEvent::MOUSE_MOVE();
}
LOST_LUA_EXPORT_END
