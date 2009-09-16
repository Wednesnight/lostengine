#include "lost/camera/Camera.h"

namespace lost
{
  namespace camera
  {
    
    Camera::Camera(const lost::math::Rect& inViewport)
    : mViewport(inViewport),
      needsUpdate(true)
    {
      mProjectionMatrix.zero();
      hasModelViewMatrix = false;
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
    
  }
}
