#include "lost/lua/bindings/LostMathVec3.h"
#include "lost/math/Vec3.h"

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
	  ]
  ];
}
LOST_LUA_EXPORT_END
