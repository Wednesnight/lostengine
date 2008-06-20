#ifndef LOST_GL_PUSHCLIENTATTRIB_H
#define LOST_GL_PUSHCLIENTATTRIB_H

#include "lost/gl/gl.h"

namespace lost
{
  namespace gl
  {
    struct PushClientAttrib
    {
      PushClientAttrib(GLbitfield mask)
      {
        glPushClientAttrib(mask);
      }

      ~PushClientAttrib()
      {
        glPopClientAttrib();
      }
    };
  }
}

#endif
