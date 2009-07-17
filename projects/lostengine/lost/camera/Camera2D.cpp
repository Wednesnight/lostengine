#include "lost/camera/Camera2D.h"
#include "lost/math/Matrix.h"

using namespace lost::math;

namespace lost
{
  namespace camera
  {

    Camera2D::Camera2D(const lost::math::Rect& inViewport)
    : Camera::Camera(inViewport)
    {
      hasModelViewMatrix = true;
      float offset = .375f;
      mModelViewMatrix = MatrixTranslation(Vec3(offset, offset, .0f));
    }

    Camera2DPtr Camera2D::create(const lost::math::Rect& inViewport)
    {
      return Camera2DPtr(new Camera2D(inViewport));
    }

    lost::math::Matrix& Camera2D::projectionMatrix()
    {
      if (needsUpdate)
      {
        mProjectionMatrix = math::MatrixOrtho(math::Rect(0, 0, mViewport.width, mViewport.height),
                                          math::Vec2(-1.0, 1.0));
        needsUpdate = false;        
      }
      return mProjectionMatrix;
    }

  }
}
