#include <boost/shared_ptr.hpp>
#include "lost/lua/State.h"
#include "lost/math/Rect.h"

#include "lost/lua/bindings/LostMathRect.h"

using namespace luabind;
using namespace lost::math;

namespace lost
{
  namespace lua
  {
    void LostMathRect(lost::lua::State& state)
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
  }
}
