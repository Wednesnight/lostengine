#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/event/Event.h"

#include "lost/lua/bindings/LostEventEvent.h"

using namespace luabind;
using namespace lost::event;

namespace lost
{
  namespace lua
  {
    void LostEventEvent(lua_State* state)
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
  }
}
