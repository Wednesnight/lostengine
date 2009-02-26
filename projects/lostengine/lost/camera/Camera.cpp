#include "lost/camera/Camera.h"
#include "lost/common/Logger.h"
#include "lost/gl/gl.h"

namespace lost
{
  namespace camera
  {
    
    Camera::Camera(const boost::shared_ptr<application::gl::Context>& inContext, const lost::math::Rect& inViewport)
    : context(inContext),
      viewport(inViewport)
    {
      DOUT("Camera::Camera()");
    }
   
    void Camera::apply()
    {
      context->makeCurrent();
      glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
    }
    
  }
}
