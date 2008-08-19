#include "lost/lua/State.h"
#include "lost/math/Vec4.h"
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace luabind;
using namespace lost;
using namespace lost::math;

namespace lost
{
  namespace lua
  {
    
    void bindLostMathVec4(lost::lua::State& state)
    {
      module(state, "lost")
      [
        namespace_("math")
        [
          class_<Vec4 >("Vec4")
            .def(constructor<>())
            .def(constructor<float, float, float, float>())
            .def("clear", &Vec4::clear)
        ]
      ];
    }

  }
}
