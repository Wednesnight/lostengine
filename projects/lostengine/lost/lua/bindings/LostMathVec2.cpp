#include "lost/lua/bindings/LostMathVec2.h"
#include "lost/math/Vec2.h"
#include <luabind/operator.hpp>

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
    .def(self * float())
    .def(self * other<Vec2>())
    .def(self + other<Vec2>())
    .def(self - other<Vec2>())
    .def(self == other<Vec2>())
    .def(tostring(self)),
    def("len", (float(*)(const Vec2&))&len),
    def("squlen", (float(*)(const Vec2&))&squlen),
    def("normalise", (Vec2&(*)(Vec2&))&normalise),
    def("perpendicular", (Vec2(*)(Vec2&))&perpendicular),
    def("angle", (float(*)(const Vec2&, const Vec2&))&angle),
    def("compare", (bool(*)(const Vec2&, const Vec2&, float))&compare)
	  ]
   ];
}
LOST_LUA_EXPORT_END
