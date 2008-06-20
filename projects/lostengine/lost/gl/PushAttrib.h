#ifndef LOST_GL_PUSHATTRIB_H
#define LOST_GL_PUSHATTRIB_H

#include "lost/gl/gl.h"

namespace lost
{
  namespace gl
  {
    struct PushAttrib
    {
      PushAttrib(GLbitfield mask)
      {
        glPushAttrib(mask);
      }

      ~PushAttrib()
      {
        glPopAttrib();
      }
    };
  }
}

#endif
