#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/application/TouchEvent.h"
#include "lost/lua/EventCast.h"
#include <luabind/iterator_policy.hpp>

#include "lost/lua/bindings/LostApplicationTouchEvent.h"

using namespace luabind;
using namespace lost::application;
using namespace lost::event;

namespace lost
{
  namespace lua
  {
    void LostApplicationTouchEvent(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<TouchEvent, Event, boost::shared_ptr<TouchEvent> >("TouchEvent")
            .def(constructor<std::string>()) 
            .def("size", &TouchEvent::size) 
            .def_readwrite("touches", &TouchEvent::touches, return_stl_iterator)
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
  }
}
