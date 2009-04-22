#include "lost/lua/lua.h"
#include "lost/application/InputEvent.h"
#include "lost/lua/EventCast.h"

#include "lost/lua/bindings/LostApplicationInputEvent.h"

using namespace luabind;
using namespace lost::event;
using namespace lost::application;

namespace lost
{
  namespace lua
  {
    void LostApplicationInputEvent(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<InputEvent, Event, lost::shared_ptr<InputEvent> >("InputEvent")
          .def(constructor<std::string>()) 
          .def_readonly("window", &InputEvent::window)
          .scope
          [
            def("cast", &lost::lua::cast<InputEvent>)
          ]
        ]
      ];
    }
  }
}
