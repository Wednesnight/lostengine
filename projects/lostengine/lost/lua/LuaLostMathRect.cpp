#include "lost/lua/LuaBindings.h"
#include "lost/math/Rect.h"

using namespace luabind;
using namespace lost::math;

LOST_LUA_EXPORT_BEGIN(LuaLostMathRect)
{
  module(state, "lost")
  [
    namespace_("math")
    [
      class_<Rect, boost::shared_ptr<Rect> >("Rect")
      .def(constructor<>()) 
      .def(constructor<float, float, float, float>())
    ]
  ];
}
LOST_LUA_EXPORT_END
