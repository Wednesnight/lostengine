#include "lost/lua/LuaBindings.h"
#include "lost/common/FpsMeter.h"

using namespace luabind;
using namespace lost::common;

LOST_LUA_EXPORT_BEGIN(LuaLostCommonFpsMeter)
{
  module(state, "lost")
  [
    namespace_("common")
    [
      class_<FpsMeter, boost::shared_ptr<FpsMeter> >("FpsMeter")
      .def(constructor<>()) 
      .def("render", &FpsMeter::render)
      .def_readwrite("width", &FpsMeter::width)
      .def_readwrite("height", &FpsMeter::height)
      .def_readwrite("alpha", &FpsMeter::alpha)
    ]
  ];
}
LOST_LUA_EXPORT_END
