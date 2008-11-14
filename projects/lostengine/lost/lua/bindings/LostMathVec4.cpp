#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/math/Vec4.h"

#include "lost/lua/bindings/LostMathVec4.h"

using namespace luabind;
using namespace lost::math;

namespace lost
{
  namespace lua
  {
    void LostMathVec4(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("math")
        [
          class_<Vec4, boost::shared_ptr<Vec4> >("Vec4")
            .def(constructor<>())
            .def(constructor<const Vec4&>())
            .def(constructor<float, float, float, float>())
            .def("clear", &Vec4::clear)
        ]
      ];
    }
  }
}
