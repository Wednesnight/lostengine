#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/common/Color.h"

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
