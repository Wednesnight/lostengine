#include "lost/lua/Luabindings.h"
#include "lost/math/Vec4.h"

using namespace luabind;
using namespace lost::math;

LOST_LUA_EXPORT_BEGIN(LuaLostMathVec4)
{
  module(state, "lost")
  [
    namespace_("math")
    [
      class_<Vec4, boost::shared_ptr<Vec4> >("Vec4")
        .def(constructor<>())
        .def(constructor<float, float, float, float>())
        .def("clear", &Vec4::clear)
    ]
  ];
}
LOST_LUA_EXPORT_END
