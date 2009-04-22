#include "lost/lua/lua.h"
#include "lost/gl/FrameBuffer.h"

#include "lost/lua/bindings/LostGLFrameBuffer.h"

using namespace boost;
using namespace luabind;
using namespace lost::gl;

namespace lost
{
  namespace lua
  {
    void LostGLFrameBuffer(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<FrameBuffer, shared_ptr<FrameBuffer> >("FrameBuffer")
            .def(constructor<ContextPtr>())
            .def("enable", &FrameBuffer::enable)
            .def("disable", &FrameBuffer::disable)
            .def("attachDepth", (void(FrameBuffer::*)(shared_ptr<Texture>))&FrameBuffer::attachDepth)
            .def("attachDepth", (void(FrameBuffer::*)(shared_ptr<RenderBuffer>))&FrameBuffer::attachDepth)
            .def("attachColor", (void(FrameBuffer::*)(int, shared_ptr<Texture>))&FrameBuffer::attachColor)
            .def("status", &FrameBuffer::status)
        ]
      ];
    }
  }
}
