#include "lost/lua/lua.h"
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/platform/Platform.h"

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

// stupid win32 __stdcall mapping
#ifdef WIN32
    #define LOST_LUA_GL_PREFIX(s) _##s

    void _glClearColor(GLclampf p1,GLclampf p2,GLclampf p3, GLclampf p4)
    {
      glClearColor(p1, p2, p3, p4);
    }

    void _glClear(GLbitfield p1)
    {
      glClear(p1);
    }

    void _glEnable(GLenum p1)
    {
      glEnable(p1);
    }

    void _glDisable(GLenum p1)
    {
      glDisable(p1);
    }

    void _glEnableClientState(GLenum p1)
    {
      glEnableClientState(p1);
    }

    void _glDisableClientState(GLenum p1)
    {
      glDisableClientState(p1);
    }

    void _glMatrixMode(GLenum p1)
    {
      glMatrixMode(p1);
    }

    void _glLoadIdentity()
    {
      glLoadIdentity();
    }

    void _glViewport(GLint p1,GLint p2,GLsizei p3,GLsizei p4)
    {
      glViewport(p1, p2, p3, p4);
    }

    void _glLineWidth(GLfloat p1)
    {
      glLineWidth(p1);
    }

    void _glTranslatef(GLfloat p1,GLfloat p2,GLfloat p3)
    {
      glTranslatef(p1, p2, p3);
    }

    void _glPushAttrib(GLbitfield p1)
    {
      glPushAttrib(p1);
    }

    void _glPopAttrib()
    {
      glPopAttrib();
    }
#else
    // not win32
    #define LOST_LUA_GL_PREFIX(s) s
#endif

    void LostGLGL(lua_State* state)
    {
      module(state, "gl")
      [
       def("GLDEBUG", &glDebug),
       def("glClearColor", &LOST_LUA_GL_PREFIX(glClearColor)),
       def("glClear", &LOST_LUA_GL_PREFIX(glClear)),
       def("glEnable", &LOST_LUA_GL_PREFIX(glEnable)),
       def("glDisable", &LOST_LUA_GL_PREFIX(glDisable)),
       def("glEnableClientState", &LOST_LUA_GL_PREFIX(glEnableClientState)),
       def("glDisableClientState", &LOST_LUA_GL_PREFIX(glDisableClientState)),
       def("glMatrixMode", &LOST_LUA_GL_PREFIX(glMatrixMode)),
       def("glLoadIdentity", &LOST_LUA_GL_PREFIX(glLoadIdentity)),
       def("glViewport", &LOST_LUA_GL_PREFIX(glViewport)),
       def("glLineWidth", &LOST_LUA_GL_PREFIX(glLineWidth)),
       def("glTranslate", &LOST_LUA_GL_PREFIX(glTranslatef)),
#if !defined(TARGET_IPHONE_SIMULATOR) && !defined(TARGET_IPHONE)
       def("glPushAttrib", &LOST_LUA_GL_PREFIX(glPushAttrib)),
       def("glPopAttrib", &LOST_LUA_GL_PREFIX(glPopAttrib))
#endif
       ];

#if !defined(TARGET_IPHONE_SIMULATOR) && !defined(TARGET_IPHONE)
      globals(state)["gl"]["GL_CLAMP"] = GL_CLAMP;
      globals(state)["gl"]["GL_DEPTH_COMPONENT"] = GL_DEPTH_COMPONENT;
      globals(state)["gl"]["GL_DEPTH_COMPONENT16"] = GL_DEPTH_COMPONENT16;
      globals(state)["gl"]["GL_DEPTH_COMPONENT24"] = GL_DEPTH_COMPONENT24;
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
