#include "lost/lua/lua.h"
#include "lost/lgl/lgl.h"

#include "lost/lua/bindings/LostLGLLGL.h"

using namespace luabind;

namespace lost
{
  namespace lua
  {
    
    void LostLGLLGL(lua_State* state)
    {
      module(state, "lgl")
      [
        def("lglOrtho", &lglOrtho)
      ];

      globals(state)["lgl"]["LGL_FRAMEBUFFER"] = LGL_FRAMEBUFFER;
      globals(state)["lgl"]["LGL_RENDERBUFFER"] = LGL_RENDERBUFFER;
      globals(state)["lgl"]["LGL_DEPTH_COMPONENT"] = LGL_DEPTH_COMPONENT;
      globals(state)["lgl"]["LGL_DEPTH_COMPONENT16"] = LGL_DEPTH_COMPONENT16;
      globals(state)["lgl"]["LGL_DEPTH_COMPONENT24"] = LGL_DEPTH_COMPONENT24;
      globals(state)["lgl"]["LGL_FRAMEBUFFER_COMPLETE"] = LGL_FRAMEBUFFER_COMPLETE;
    }
  }
}
