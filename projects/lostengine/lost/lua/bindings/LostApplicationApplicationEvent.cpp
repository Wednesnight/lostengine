#include "lost/lua/lua.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/lua/EventCast.h"

#include "lost/lua/bindings/LostApplicationApplicationEvent.h"

using namespace luabind;
using namespace lost::event;
using namespace lost::application;

namespace lost
{
  namespace lua
  {
    void LostApplicationApplicationEvent(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<ApplicationEvent, Event, lost::shared_ptr<ApplicationEvent> >("ApplicationEvent")
          .def(constructor<std::string>()) 
          .scope
          [
            def("cast", &lost::lua::cast<ApplicationEvent>)
          ]
         ]
      ];
      globals(state)["lost"]["application"]["ApplicationEvent"]["RUN"] = ApplicationEvent::RUN();
      globals(state)["lost"]["application"]["ApplicationEvent"]["QUIT"] = ApplicationEvent::QUIT();
    }
  }
}
