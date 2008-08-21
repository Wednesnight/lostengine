#include "lost/lua/Luabindings.h"
#include "lost/application/ResizeEvent.h"

using namespace luabind;
using namespace lost::event;
using namespace lost::application;

LOST_LUA_EXPORT_BEGIN(LuaLostApplicationResizeEvent)
{
  module(state, "lost")
  [
   namespace_("application")
   [
    class_<ResizeEvent, boost::shared_ptr<ResizeEvent>, boost::shared_ptr<Event> >("ResizeEvent")
    .def(constructor<std::string>()) 
    .def_readwrite("width", &ResizeEvent::width)
    .def_readwrite("height", &ResizeEvent::height)
    ]
   ];
  
  globals(state)["lost"]["application"]["ResizeEvent"]["MAIN_WINDOW_RESIZE"] = ResizeEvent::MAIN_WINDOW_RESIZE();
}
LOST_LUA_EXPORT_END
