#include "lost/lua/LuaBindings.h"
#include "lost/application/TouchEvent.h"
#include "lost/lua/LuaEventCast.h"

using namespace luabind;
using namespace lost::application;
using namespace lost::event;

LOST_LUA_EXPORT_BEGIN(LuaLostApplicationTouchEvent)
{
  module(state, "lost")
  [
    namespace_("application")
    [
      class_<TouchEvent, boost::shared_ptr<TouchEvent>, boost::shared_ptr<Event> >("TouchEvent")
        .def(constructor<std::string>()) 
        .def("size", &TouchEvent::size) 
        .def("get", &TouchEvent::get) 
//        .def_readwrite("touches", &TouchEvent::touches)
        .scope
        [
          class_<TouchEvent::Touch, boost::shared_ptr<TouchEvent::Touch> >("Touch")
            .def(constructor<>()) 
            .def_readwrite("location", &TouchEvent::Touch::location)
            .def_readwrite("tapCount", &TouchEvent::Touch::tapCount)
            .def_readwrite("timeStamp", &TouchEvent::Touch::timeStamp),

          def("cast", &lost::lua::cast<TouchEvent>)
        ]
     ]
  ];
  globals(state)["lost"]["application"]["TouchEvent"]["TOUCHES_BEGAN"] = TouchEvent::TOUCHES_BEGAN();
  globals(state)["lost"]["application"]["TouchEvent"]["TOUCHES_MOVED"] = TouchEvent::TOUCHES_MOVED();
  globals(state)["lost"]["application"]["TouchEvent"]["TOUCHES_ENDED"] = TouchEvent::TOUCHES_ENDED();
  globals(state)["lost"]["application"]["TouchEvent"]["TOUCHES_CANCELLED"] = TouchEvent::TOUCHES_CANCELLED();
}
LOST_LUA_EXPORT_END
