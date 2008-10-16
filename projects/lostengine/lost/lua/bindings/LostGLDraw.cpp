#include "lost/lua/lua.h"
#include "lost/gl/Draw.h"
#include "lost/math/Rect.h"
#include "lost/common/Color.h"

#include "lost/lua/bindings/LostGLDraw.h"

using namespace luabind;
using namespace lost::gl;

namespace lost
{
  namespace lua
  {
    void LostGLDraw(lua_State* state)
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
  }
}
