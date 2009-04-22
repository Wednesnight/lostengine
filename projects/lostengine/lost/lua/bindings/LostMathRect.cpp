#include "lost/lua/lua.h"
#include "lost/math/Rect.h"
#include <luabind/operator.hpp>

#include "lost/lua/bindings/LostMathRect.h"

using namespace luabind;
using namespace lost::math;

namespace lost
{
  namespace lua
  {
    void LostMathRect(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("math")
        [
          class_<Rect, lost::shared_ptr<Rect> >("Rect")
          .def(constructor<>()) 
          .def(constructor<const Rect&>()) 
          .def(constructor<float, float, float, float>())
          .def("contains", &Rect::contains)
          .def(tostring(self))
          .def("maxX", &Rect::maxX)
          .def("maxY", &Rect::maxY)
          .def_readwrite("x", &Rect::x)
          .def_readwrite("y", &Rect::y)
          .def_readwrite("width", &Rect::width)
          .def_readwrite("height", &Rect::height)
        ]
      ];
    }
  }
}
