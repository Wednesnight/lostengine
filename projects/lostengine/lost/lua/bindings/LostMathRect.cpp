#include "lost/lua/bindings/LostMathRect.h"
#include "lost/math/Rect.h"

using namespace luabind;
using namespace lost::math;

LOST_LUA_EXPORT_BEGIN(LostMathRect)
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
