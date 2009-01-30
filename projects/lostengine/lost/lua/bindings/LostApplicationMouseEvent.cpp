#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/MouseButton.h"
#include "lost/lua/EventCast.h"

#include "lost/lua/bindings/LostApplicationMouseEvent.h"

using namespace luabind;
using namespace lost::event;
using namespace lost::application;

namespace lost
{
  namespace lua
  {
    void LostApplicationMouseEvent(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<MouseEvent, InputEvent, boost::shared_ptr<MouseEvent> >("MouseEvent")
          .def(constructor<std::string>()) 
          .def_readwrite("pos", &MouseEvent::pos)
          .def_readwrite("absPos", &MouseEvent::absPos)
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

      globals(state)["lost"]["application"]["MB_UNKNOWN"] = MB_UNKNOWN;

      globals(state)["lost"]["application"]["MB_LEFT"]   = MB_LEFT;
      globals(state)["lost"]["application"]["MB_RIGHT"]  = MB_RIGHT;
      globals(state)["lost"]["application"]["MB_MIDDLE"] = MB_MIDDLE;
    }
  }
}
