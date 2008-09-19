#ifndef LOST_GL_STATE_H
#define LOST_GL_STATE_H

#include <boost/shared_ptr.hpp>
#include "lost/gl/gl.h"
#include "lost/common/Color.h"

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

      lost::common::Color clearColor;

      // client-side attributes
      bool normalArray;
      bool vertexArray;
    };

  }
}

#endif
