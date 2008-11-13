#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/application/ResizeEvent.h"
#include "lost/lua/EventCast.h"

#include "lost/lua/bindings/LostApplicationResizeEvent.h"

using namespace luabind;
using namespace lost::event;
using namespace lost::application;

namespace lost
{
  namespace lua
  {
    void LostApplicationResizeEvent(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<ResizeEvent, Event, boost::shared_ptr<ResizeEvent> >("ResizeEvent")
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
  }
}
