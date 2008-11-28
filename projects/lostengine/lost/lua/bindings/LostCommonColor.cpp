#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/common/Color.h"
#include <luabind/operator.hpp>

#include "lost/lua/bindings/LostCommonColor.h"

using namespace luabind;
using namespace lost::common;

namespace lost
{
  namespace lua
  {
    void LostCommonColor(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("common")
        [
          class_<Color, boost::shared_ptr<Color> >("Color")
          .def(constructor<>()) 
          .def(constructor<float, float, float>())
          .def(constructor<float, float, float, float>())
          .def(constructor<const Color&>())
          .def(tostring(self))
          .property("r", (float(Color::*)() const)&Color::r, (void(Color::*)(float))&Color::r)
          .property("g", (float(Color::*)() const)&Color::g, (void(Color::*)(float))&Color::g)
          .property("b", (float(Color::*)() const)&Color::b, (void(Color::*)(float))&Color::b)
          .property("a", (float(Color::*)() const)&Color::a, (void(Color::*)(float))&Color::a)
        ]
      ];
/*
      globals(state)["lost"]["common"]["transparentColor"] = Color(transparentColor);
      globals(state)["lost"]["common"]["whiteColor"]       = Color(whiteColor);
      globals(state)["lost"]["common"]["blackColor"]       = Color(blackColor);
      globals(state)["lost"]["common"]["redColor"]         = Color(redColor);
      globals(state)["lost"]["common"]["greenColor"]       = Color(greenColor);
      globals(state)["lost"]["common"]["blueColor"]        = Color(blueColor);
      globals(state)["lost"]["common"]["yellowColor"]      = Color(yellowColor);
*/
    }
  }
}
