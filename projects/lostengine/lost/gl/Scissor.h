#ifndef LOST_GL_SCISSOR_H
#define LOST_GL_SCISSOR_H

#include "lost/gl/gl.h"
#include "lost/math/Rect.h"

namespace lost
{

namespace gl
{

  struct Scissor
  {
    const lost::math::Rect& r;
    Scissor(const lost::math::Rect& rct) : r(rct)
    {
      glPushAttrib(GL_SCISSOR_BIT);
   		glScissor((int)rct.x, (int)rct.y, (int)rct.width, (int)rct.height);
    }

    ~Scissor()
    {
   		glPopAttrib();
    }
  };

}

}

#endif

