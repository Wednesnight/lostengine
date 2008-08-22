#include "lost/lua/Luabindings.h"
#include "lost/application/AccelerometerEvent.h"
#include "lost/lua/LuaEventCast.h"

using namespace luabind;
using namespace lost::application;
using namespace lost::event;

LOST_LUA_EXPORT_BEGIN(LuaLostApplicationAccelerometerEvent)
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
LOST_LUA_EXPORT_END
