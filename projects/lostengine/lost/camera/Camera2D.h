#ifndef LOST_CAMERA_CAMERA2D_H
#define LOST_CAMERA_CAMERA2D_H

#include "lost/camera/Camera.h"

namespace lost
{
  namespace camera
  {
    struct Camera2D;
    typedef lost::shared_ptr<Camera2D> Camera2DPtr;

    struct Camera2D : public Camera
    {
      Camera2D(const lost::math::Rect& inViewport);

      lost::math::Matrix& matrix();
    };

  }
}

#endif
