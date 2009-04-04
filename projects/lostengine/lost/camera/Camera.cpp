#include "lost/camera/Camera.h"
#include "lost/common/Logger.h"
#include "lost/gl/gl.h"

namespace lost
{
  namespace camera
  {
    
    Camera::Camera(gl::ContextPtr inContext, const lost::math::Rect& inViewport)
    : context(inContext),
      viewport(inViewport)
    {
    }
   
    void Camera::apply()
    {
      glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
    }
    
  }
}
