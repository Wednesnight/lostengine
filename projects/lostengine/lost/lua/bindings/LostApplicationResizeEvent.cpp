#include "lost/lua/bindings/LostApplicationResizeEvent.h"
#include "lost/application/ResizeEvent.h"
#include "lost/lua/EventCast.h"

using namespace luabind;
using namespace lost::event;
using namespace lost::application;

LOST_LUA_EXPORT_BEGIN(LostApplicationResizeEvent)
{
  module(state, "lost")
  [
    namespace_("application")
    [
      class_<ResizeEvent, boost::shared_ptr<ResizeEvent>, boost::shared_ptr<Event> >("ResizeEvent")
      .def(constructor<std::string>()) 
      .def_readwrite("width", &ResizeEvent::width)
      .def_readwrite("height", &ResizeEvent::height)
      .scope
      [
        def("cast", &lost::lua::cast<ResizeEvent>)
      ]
    ]
  ];
  globals(state)["lost"]["application"]["ResizeEvent"]["MAIN_WINDOW_RESIZE"] = ResizeEvent::MAIN_WINDOW_RESIZE();
}
LOST_LUA_EXPORT_END
