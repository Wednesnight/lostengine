#ifndef LOST_CAMERA_CAMERA_H
#define LOST_CAMERA_CAMERA_H

#include "lost/gl/Context.h"
#include "lost/math/Rect.h"

namespace lost
{
  namespace camera
  {
    struct Camera;
    typedef boost::shared_ptr<Camera> CameraPtr;
    struct Camera
    {
    public:
      gl::ContextPtr context;
      lost::math::Rect viewport;

      Camera(gl::ContextPtr inContext, const lost::math::Rect& inViewport);
      
      virtual void apply();
    };

  }
}

#endif
