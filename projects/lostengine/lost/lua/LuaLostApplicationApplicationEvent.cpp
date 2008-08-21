#include "lost/lua/Luabindings.h"
#include "lost/application/ApplicationEvent.h"

using namespace luabind;
using namespace lost::event;
using namespace lost::application;

LOST_LUA_EXPORT_BEGIN(LuaLostApplicationApplicationEvent)
{
  module(state, "lost")
  [
   namespace_("application")
   [
    class_<ApplicationEvent, boost::shared_ptr<ApplicationEvent>, boost::shared_ptr<Event> >("ApplicationEvent")
    .def(constructor<std::string>()) 
    ]
   ];
  globals(state)["lost"]["application"]["ApplicationEvent"]["PREINIT"] = ApplicationEvent::PREINIT();
  globals(state)["lost"]["application"]["ApplicationEvent"]["INIT"] = ApplicationEvent::INIT();
  globals(state)["lost"]["application"]["ApplicationEvent"]["RUN"] = ApplicationEvent::RUN();
  globals(state)["lost"]["application"]["ApplicationEvent"]["QUIT"] = ApplicationEvent::QUIT();
}
LOST_LUA_EXPORT_END
