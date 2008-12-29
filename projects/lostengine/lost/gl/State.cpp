#include <boost/shared_ptr.hpp>
#include "lost/gl/State.h"

namespace lost
{
  namespace gl
  {

    State::State()
    : alphaTest(false),
      depthTest(false),
      texture2D(false),
      blend(false),
      blendSrc(GL_SRC_ALPHA),
      blendDest(GL_ONE_MINUS_SRC_ALPHA),
      clearColor(lost::common::blackColor),
      normalArray(false),
      vertexArray(false),
      textureCoordArray(false),
      viewport(math::Rect(0,0,0,0))
    {
    }

  }
}
