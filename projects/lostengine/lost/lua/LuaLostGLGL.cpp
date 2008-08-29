#include "lost/lua/LuaBindings.h"
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"

using namespace luabind;

void glDebug()
{
  GLDEBUG;
}

LOST_LUA_EXPORT_BEGIN(LuaLostGLGL)
{
  module(state, "gl")
  [
   def("GLDEBUG", &glDebug),
   def("glClearColor", &glClearColor),
   def("glClear", &glClear),
   def("glEnable", &glEnable),
   def("glDisable", &glDisable),
   def("glEnableClientState", &glEnableClientState),
   def("glDisableClientState", &glDisableClientState),
   def("glMatrixMode", &glMatrixMode),
   def("glLoadIdentity", &glLoadIdentity),
   def("glViewport", &glViewport)
   ];
  globals(state)["gl"]["GL_VERTEX_ARRAY"] = GL_VERTEX_ARRAY;
  globals(state)["gl"]["GL_TEXTURE_2D"] = GL_TEXTURE_2D;
  globals(state)["gl"]["GL_DEPTH_TEST"] = GL_DEPTH_TEST;
  globals(state)["gl"]["GL_COLOR_BUFFER_BIT"] = GL_COLOR_BUFFER_BIT;
  globals(state)["gl"]["GL_DEPTH_BUFFER_BIT"] = GL_DEPTH_BUFFER_BIT;
  globals(state)["gl"]["GL_MODELVIEW"] = GL_MODELVIEW;
}
LOST_LUA_EXPORT_END
