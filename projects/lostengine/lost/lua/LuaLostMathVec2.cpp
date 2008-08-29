#include "lost/lua/Luabindings.h"
#include "lost/math/Vec2.h"

using namespace luabind;
using namespace lost::math;

LOST_LUA_EXPORT_BEGIN(LuaLostMathVec2)
{
  module(state, "lost")
  [
   namespace_("math")
   [
    class_<Vec2, boost::shared_ptr<Vec2> >("Vec2")
    .def(constructor<>())
    .def(constructor<float, float>())
    .def("zero", (void(Vec2::*)()) &Vec2::zero)
	  ]
   ];
}
LOST_LUA_EXPORT_END
