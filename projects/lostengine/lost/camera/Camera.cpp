#include "lost/camera/Camera.h"

namespace lost
{
  namespace camera
  {
    
    Camera::Camera(const lost::math::Rect& inViewport)
    : m_viewport(inViewport),
      needsUpdate(true)
    {
      m_matrix.zero();
    }
    
    lost::math::Rect& Camera::viewport(const lost::math::Rect& inViewport)
    {
      if (m_viewport != inViewport)
      {
        m_viewport = inViewport;
        needsUpdate = true;
      }
      return m_viewport;
    }

    lost::math::Rect& Camera::viewport()
    {
      return m_viewport;
    }
    
  }
}
