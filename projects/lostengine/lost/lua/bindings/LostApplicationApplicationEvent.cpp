#include <boost/shared_ptr.hpp>
#include "lost/lua/State.h"
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
    void LostApplicationApplicationEvent(lost::lua::State& state)
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
  }
}
