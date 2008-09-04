#include "lost/lua/bindings/LostMathVec2.h"
#include "lost/math/Vec2.h"

using namespace luabind;
using namespace lost::math;

LOST_LUA_EXPORT_BEGIN(LostMathVec2)
{
  module(state, "lost")
  [
   namespace_("math")
   [
//    class_<Vec2, boost::shared_ptr<Vec2> >("Vec2")
    class_<Vec2>("Vec2")
    .def(constructor<>())
    .def(constructor<float, float>())
    .def("zero", (void(Vec2::*)()) &Vec2::zero)
    .def_readwrite("x", &Vec2::x)
    .def_readwrite("y", &Vec2::y)
	  ]
   ];
}
LOST_LUA_EXPORT_END
