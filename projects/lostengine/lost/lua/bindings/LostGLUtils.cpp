#include "lost/lua/State.h"
#include "lost/gl/Utils.h"
#include "lost/math/Vec2.h"

#include "lost/lua/bindings/LostGLUtils.h"

using namespace luabind;
using namespace lost::gl::utils;

namespace lost
{
  namespace lua
  {
    void LostGLUtils(lost::lua::State& state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          namespace_("utils")
          [
            def("set2DProjection", &set2DProjection)
          ]
        ]
      ];
    }
  }
}
