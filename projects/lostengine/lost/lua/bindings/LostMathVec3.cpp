#include "lost/lua/bindings/LostMathVec3.h"
#include "lost/math/Vec3.h"
#include <luabind/operator.hpp>

using namespace luabind;
using namespace lost::math;

LOST_LUA_EXPORT_BEGIN(LostMathVec3)
{
  module(state, "lost")
  [
    namespace_("math")
    [
      class_<Vec3, boost::shared_ptr<Vec3> >("Vec3")
	      .def(constructor<>())
	      .def(constructor<float, float, float>())
	      .def("zero", (void(Vec3::*)()) &Vec3::zero)
        .def(self * float())
        .def(self * other<Vec3>())
        .def(self + other<Vec3>())
        .def(self - other<Vec3>())
        .def(self == other<Vec3>())
        .def(tostring(self)),
         def("len", (float(*)(const Vec3&))&len),
         def("squlen", (float(*)(const Vec3&))&squlen),
         def("normalise", (Vec3&(*)(Vec3&))&normalise),
         def("angle", (float(*)(const Vec3&, const Vec3&))&angle),
         def("compare", (bool(*)(const Vec3&, const Vec3&, float))&compare)
     ]
  ];
}
LOST_LUA_EXPORT_END
