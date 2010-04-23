#ifndef LOST_CAMERA_CAMERA_H
#define LOST_CAMERA_CAMERA_H

#include "lost/math/Rect.h"
#include "lost/math/Matrix.h"
#include "lost/platform/shared_ptr.h"

namespace lost
{
  namespace camera
  {
    struct Camera;
    typedef lost::shared_ptr<Camera> CameraPtr;
    struct Camera
    {
    protected:
      lost::math::Rect mViewport;
      math::Vec2       mDepth;

      lost::math::Matrix mViewMatrix;
      lost::math::Matrix mProjectionMatrix;
      
      virtual void update() = 0;
    public:
      bool needsUpdate;

      Camera(const lost::math::Rect& inViewport);

      lost::math::Rect& viewport(const lost::math::Rect& inViewport);
      lost::math::Rect& viewport();

      math::Vec2 depth();
      void depth(const math::Vec2& inDepth);

      lost::math::Matrix& viewMatrix();
      lost::math::Matrix& projectionMatrix();
    };

  }
}

#endif
