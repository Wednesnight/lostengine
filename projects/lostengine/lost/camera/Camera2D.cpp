#include "lost/camera/Camera2D.h"
#include "lost/lgl/lglu.h"

namespace lost
{
  namespace camera
  {

    Camera2D::Camera2D(const boost::shared_ptr<application::gl::Context>& inContext, const lost::math::Rect& inViewport)
    : Camera::Camera(inContext, inViewport),
      space(-0.5, -0.5, inViewport.width - 0.5, inViewport.height - 0.5)
    {
    }

    void Camera2D::apply()
    {
      Camera::apply();
      lgluOrtho2D(space.x, space.width, space.y, space.height);
    }

  }
}
