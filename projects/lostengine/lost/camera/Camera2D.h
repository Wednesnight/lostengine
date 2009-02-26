#ifndef LOST_CAMERA_CAMERA2D_H
#define LOST_CAMERA_CAMERA2D_H

#include "lost/camera/Camera.h"

namespace lost
{
  namespace camera
  {

    struct Camera2D : public Camera
    {
    private:
      lost::math::Rect space;
    public:
      Camera2D(const boost::shared_ptr<application::gl::Context>& inContext, const lost::math::Rect& inViewport);

      virtual void apply();
    };

  }
}

#endif
