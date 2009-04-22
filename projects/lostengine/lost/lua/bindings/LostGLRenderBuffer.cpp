#include "lost/lua/lua.h"
#include "lost/gl/RenderBuffer.h"

#include "lost/lua/bindings/LostGLRenderBuffer.h"

using namespace boost;
using namespace luabind;
using namespace lost::gl;

namespace lost
{
  namespace lua
  {
    void LostGLRenderBuffer(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<RenderBuffer, shared_ptr<RenderBuffer> >("RenderBuffer")
            .def(constructor<>())
            .def("enable", &RenderBuffer::enable)
            .def("disable", &RenderBuffer::disable)
            .def("storage", &RenderBuffer::storage)
        ]
      ];
    }
  }
}
