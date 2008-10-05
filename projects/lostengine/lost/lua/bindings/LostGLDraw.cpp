#include "lost/lua/bindings/LostGLDraw.h"
#include "lost/gl/Draw.h"
#include "lost/math/Rect.h"
#include "lost/common/Color.h"

using namespace luabind;
using namespace lost::gl;

LOST_LUA_EXPORT_BEGIN(LostGLDraw)
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
