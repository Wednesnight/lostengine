#include "lost/lua/bindings/LostCommonColor.h"
#include "lost/common/Color.h"

using namespace luabind;
using namespace lost::common;

LOST_LUA_EXPORT_BEGIN(LostCommonColor)
{
  module(state, "lost")
  [
    namespace_("common")
    [
      class_<Color, boost::shared_ptr<Color> >("Color")
      .def(constructor<>()) 
      .def(constructor<float, float, float>())
      .def(constructor<float, float, float, float>())
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
LOST_LUA_EXPORT_END
