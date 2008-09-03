#include "lost/lua/LuaBindings.h"
#include "lost/gl/Draw.h"

using namespace luabind;
using namespace lost::gl;

LOST_LUA_EXPORT_BEGIN(LuaLostGLDraw)
{
  module(state, "lost")
  [
    namespace_("gl")
    [
      def("setColor", &setColor),
      def("drawRectOutline", &drawRectOutline),
      def("drawRectFilled", &drawRectFilled)
    ]
  ];
}
LOST_LUA_EXPORT_END
