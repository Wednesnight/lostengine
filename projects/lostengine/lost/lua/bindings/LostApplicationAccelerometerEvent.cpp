#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/application/AccelerometerEvent.h"
#include "lost/lua/EventCast.h"

#include "lost/lua/bindings/LostApplicationAccelerometerEvent.h"

using namespace luabind;
using namespace lost::application;
using namespace lost::event;

namespace lost
{
  namespace lua
  {
    void LostApplicationAccelerometerEvent(lua_State* state)
    {
      module(state, "lost")
      [
       namespace_("application")
       [
         class_<AccelerometerEvent, boost::shared_ptr<AccelerometerEvent>, boost::shared_ptr<Event> >("AccelerometerEvent")
           .def(constructor<std::string>()) 
           .def_readwrite("x", &AccelerometerEvent::x)
           .def_readwrite("y", &AccelerometerEvent::y)
           .def_readwrite("z", &AccelerometerEvent::z)
           .def_readwrite("timeStamp", &AccelerometerEvent::timeStamp)
           .scope
           [
             def("cast", &lost::lua::cast<AccelerometerEvent>)
           ]
         ]
       ];
      globals(state)["lost"]["application"]["AccelerometerEvent"]["DEVICE_ACCELERATED"] = AccelerometerEvent::DEVICE_ACCELERATED();
    }
  }
}
