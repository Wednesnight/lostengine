#include "lost/lua/bindings/LostApplicationApplicationEvent.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/lua/EventCast.h"

using namespace luabind;
using namespace lost::event;
using namespace lost::application;

LOST_LUA_EXPORT_BEGIN(LostApplicationApplicationEvent)
{
  module(state, "lost")
  [
    namespace_("application")
    [
      class_<ApplicationEvent, boost::shared_ptr<ApplicationEvent>, boost::shared_ptr<Event> >("ApplicationEvent")
      .def(constructor<std::string>()) 
      .scope
      [
        def("cast", &lost::lua::cast<ApplicationEvent>)
      ]
     ]
  ];
  globals(state)["lost"]["application"]["ApplicationEvent"]["PREINIT"] = ApplicationEvent::PREINIT();
  globals(state)["lost"]["application"]["ApplicationEvent"]["INIT"] = ApplicationEvent::INIT();
  globals(state)["lost"]["application"]["ApplicationEvent"]["RUN"] = ApplicationEvent::RUN();
  globals(state)["lost"]["application"]["ApplicationEvent"]["QUIT"] = ApplicationEvent::QUIT();
}
LOST_LUA_EXPORT_END
