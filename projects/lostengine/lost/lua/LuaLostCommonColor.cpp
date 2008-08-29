#include "lost/lua/LuaBindings.h"
#include "lost/common/Color.h"

using namespace luabind;
using namespace lost::common;

LOST_LUA_EXPORT_BEGIN(LuaLostCommonColor)
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
  globals(state)["lost"]["common"]["transparentColor"] = transparentColor;
  globals(state)["lost"]["common"]["whiteColor"]       = whiteColor;
  globals(state)["lost"]["common"]["blackColor"]       = blackColor;
  globals(state)["lost"]["common"]["redColor"]         = redColor;
  globals(state)["lost"]["common"]["greenColor"]       = greenColor;
  globals(state)["lost"]["common"]["blueColor"]        = blueColor;
  globals(state)["lost"]["common"]["yellowColor"]      = yellowColor;
*/
}
LOST_LUA_EXPORT_END
