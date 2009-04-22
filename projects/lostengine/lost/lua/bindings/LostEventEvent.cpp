#include "lost/lua/lua.h"
#include "lost/event/Event.h"

#include "lost/lua/bindings/LostEventEvent.h"

using namespace luabind;
using namespace lost::event;

namespace lost
{
  namespace lua
  {
    lost::shared_ptr<Event> asSharedPtr(Event* event)
    {
      return event->shared_from_this();
    }

    void LostEventEvent(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("event")
        [
          class_<Event, lost::shared_ptr<Event> >("Event")
           .def(constructor<std::string>()) 
           .def("asSharedPtr", &asSharedPtr)
           .def_readwrite("type", &Event::type)
        ]
      ];
    }
  }
}
