#include "lost/lua/LuaBindings.h"
#include "lost/gl/Utils.h"

using namespace luabind;
using namespace lost::gl::utils;

LOST_LUA_EXPORT_BEGIN(LuaLostGLUtils)
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
LOST_LUA_EXPORT_END
