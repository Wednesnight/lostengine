#include "lost/lua/State.h"
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
    void LostGLGL(lost::lua::State& state)
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
       def("glViewport", &glViewport),
       def("glLineWidth", &glLineWidth)
       ];
      globals(state)["gl"]["GL_VERTEX_ARRAY"] = GL_VERTEX_ARRAY;
      globals(state)["gl"]["GL_TEXTURE_2D"] = GL_TEXTURE_2D;
      globals(state)["gl"]["GL_DEPTH_TEST"] = GL_DEPTH_TEST;
      globals(state)["gl"]["GL_COLOR_BUFFER_BIT"] = GL_COLOR_BUFFER_BIT;
      globals(state)["gl"]["GL_DEPTH_BUFFER_BIT"] = GL_DEPTH_BUFFER_BIT;
      globals(state)["gl"]["GL_MODELVIEW"] = GL_MODELVIEW;
//      globals(state)["gl"]["GL_POLYGON_SMOOTH"] = GL_POLYGON_SMOOTH;
      globals(state)["gl"]["GL_LINE_SMOOTH"] = GL_LINE_SMOOTH;
      globals(state)["gl"]["GL_POINT_SMOOTH"] = GL_POINT_SMOOTH;
    }
  }
}
