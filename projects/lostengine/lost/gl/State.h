#ifndef LOST_GL_STATE_H
#define LOST_GL_STATE_H

#include <boost/shared_ptr.hpp>
#include "lost/gl/gl.h"

namespace lost
{
  namespace gl
  {

    struct State
    {
      // server-side attributes
      bool alphaTest;
      bool depthTest;
      bool texture2D;

      // client-side attributes
      bool normalArray;
      bool vertexArray;
    };

  }
}

#endif
