#ifndef LOST_CAMERA_CAMERA_H
#define LOST_CAMERA_CAMERA_H

#include "lost/math/Rect.h"
#include "lost/math/Matrix.h"

namespace lost
{
  namespace camera
  {
    struct Camera;
    typedef lost::shared_ptr<Camera> CameraPtr;
    struct Camera
    {
      lost::math::Matrix mProjectionMatrix;
      lost::math::Matrix mModelViewMatrix;
      lost::math::Rect   mViewport;
      bool hasModelViewMatrix;
      
      bool needsUpdate;

      Camera(const lost::math::Rect& inViewport);

      virtual lost::math::Matrix& projectionMatrix() = 0;

      lost::math::Rect& viewport(const lost::math::Rect& inViewport);
      lost::math::Rect& viewport();
    };

  }
}

#endif
