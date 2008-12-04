#include "lost/lua/lua.h"
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"

#include "lost/lua/bindings/LostGLGL.h"

using namespace luabind;

void glDebug()
{
  GLDEBUG;
}

namespace lost
{
  namespace lua
  {
    void LostGLGL(lua_State* state)
    {
      module(state, "gl")
      [
       def("GLDEBUG", &glDebug),
       def("glClearColor", (void(*)(GLclampf,GLclampf,GLclampf,GLclampf))&glClearColor),
       def("glClear", (void(*)(GLbitfield))&glClear),
       def("glEnable", (void(*)(GLenum))&glEnable),
       def("glDisable", (void(*)(GLenum))&glDisable),
       def("glEnableClientState", (void(*)(GLenum))&glEnableClientState),
       def("glDisableClientState", (void(*)(GLenum))&glDisableClientState),
       def("glMatrixMode", (void(*)(GLenum))&glMatrixMode),
       def("glLoadIdentity", (void(*)(void))&glLoadIdentity),
       def("glViewport", (void(*)(GLint,GLint,GLsizei,GLsizei))&glViewport),
       def("glLineWidth", (void(*)(GLfloat))&glLineWidth),
       def("glTranslate", (void(*)(GLfloat,GLfloat,GLfloat))&glTranslatef),
#if !defined(TARGET_IPHONE_SIMULATOR) && !defined(TARGET_IPHONE)
       def("glPushAttrib", (void(*)(GLbitfield))&glPushAttrib),
       def("glPopAttrib", (void(*)(void))&glPopAttrib)
#endif
       ];

#if !defined(TARGET_IPHONE_SIMULATOR) && !defined(TARGET_IPHONE)
      globals(state)["gl"]["GL_CLAMP"] = GL_CLAMP;
      globals(state)["gl"]["GL_DEPTH_COMPONENT"] = GL_DEPTH_COMPONENT;
      globals(state)["gl"]["GL_DEPTH_COMPONENT32"] = GL_DEPTH_COMPONENT32;
      globals(state)["gl"]["GL_FRAMEBUFFER_COMPLETE_EXT"] = GL_FRAMEBUFFER_COMPLETE_EXT;
      globals(state)["gl"]["GL_RGBA8"] = GL_RGBA8;
      globals(state)["gl"]["GL_SCISSOR_BIT"] = GL_SCISSOR_BIT;
      globals(state)["gl"]["GL_VIEWPORT_BIT"] = GL_VIEWPORT_BIT;
#endif
      globals(state)["gl"]["GL_COLOR_BUFFER_BIT"] = GL_COLOR_BUFFER_BIT;
      globals(state)["gl"]["GL_DEPTH_BUFFER_BIT"] = GL_DEPTH_BUFFER_BIT;
      globals(state)["gl"]["GL_DEPTH_TEST"] = GL_DEPTH_TEST;
      globals(state)["gl"]["GL_FLOAT"] = GL_FLOAT;
      globals(state)["gl"]["GL_LINE_SMOOTH"] = GL_LINE_SMOOTH;
      globals(state)["gl"]["GL_LINEAR"] = GL_LINEAR;
      globals(state)["gl"]["GL_MODELVIEW"] = GL_MODELVIEW;
      globals(state)["gl"]["GL_NEAREST"] = GL_NEAREST;
      globals(state)["gl"]["GL_ONE_MINUS_SRC_ALPHA"] = GL_ONE_MINUS_SRC_ALPHA;
      globals(state)["gl"]["GL_POINT_SMOOTH"] = GL_POINT_SMOOTH;
      globals(state)["gl"]["GL_RGBA"] = GL_RGBA;
      globals(state)["gl"]["GL_SRC_ALPHA"] = GL_SRC_ALPHA;
      globals(state)["gl"]["GL_TEXTURE_2D"] = GL_TEXTURE_2D;
      globals(state)["gl"]["GL_TRIANGLES"] = GL_TRIANGLES;
      globals(state)["gl"]["GL_UNSIGNED_BYTE"] = GL_UNSIGNED_BYTE;
      globals(state)["gl"]["GL_VERTEX_ARRAY"] = GL_VERTEX_ARRAY;
      globals(state)["gl"]["GL_MODELVIEW"] = GL_MODELVIEW;
    }
  }
}
