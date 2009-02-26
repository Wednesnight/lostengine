#ifndef LOST_CAMERA_CAMERA_H
#define LOST_CAMERA_CAMERA_H

#include "lost/application/gl/Context.h"
#include "lost/math/Rect.h"

namespace lost
{
  namespace camera
  {

    struct Camera
    {
    public:
      boost::shared_ptr<application::gl::Context> context;
      lost::math::Rect viewport;

      Camera(const boost::shared_ptr<application::gl::Context>& inContext, const lost::math::Rect& inViewport);
      
      virtual void apply();
    };

  }
}

#endif
