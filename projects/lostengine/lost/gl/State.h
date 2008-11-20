#ifndef LOST_GL_STATE_H
#define LOST_GL_STATE_H

#include "lost/forward/boost/shared_ptr.hpp"
#include "lost/gl/gl.h"
#include "lost/common/Color.h"

namespace lost
{
  namespace gl
  {
    struct State
    {
      // this constructor exists only for the sake of completeness and shouldn't really be used.
      // You should always use the current context to clone/copy the current render state and then modify it to your needs
      // Also, don't forget to finetune your renderstates to minimise the changes between them.
      State();

      // server-side attributes
      bool alphaTest;
      bool depthTest;
      bool texture2D;
      bool blend;
      GLenum blendSrc;
      GLenum blendDest;

      lost::common::Color clearColor;

      // client-side attributes
      bool normalArray;
      bool vertexArray;
      bool textureCoordArray;
    };
  }
}

#endif
