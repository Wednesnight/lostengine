#include "lost/camera/Camera.h"

namespace lost
{
  namespace camera
  {
    
    Camera::Camera(const lost::math::Rect& inViewport)
    : mViewport(inViewport),
      needsUpdate(true)
    {
      mViewMatrix.initIdentity();
      mProjectionMatrix.initIdentity();
    }
    
    lost::math::Rect& Camera::viewport(const lost::math::Rect& inViewport)
    {
      if (mViewport != inViewport)
      {
        mViewport = inViewport;
        needsUpdate = true;
      }
      return mViewport;
    }

    lost::math::Rect& Camera::viewport()
    {
      return mViewport;
    }

    lost::math::Matrix& Camera::viewMatrix()
    {
      if (needsUpdate) update();
      return mViewMatrix;
    }

    lost::math::Matrix& Camera::projectionMatrix()
    {
      if (needsUpdate) update();
      return mProjectionMatrix;
    }

  }
}
