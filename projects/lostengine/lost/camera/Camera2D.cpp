#include "lost/camera/Camera2D.h"
#include "lost/lgl/lglu.h"
#include "lost/math/Matrix.h"

namespace lost
{
  namespace camera
  {

    Camera2D::Camera2D(const lost::math::Rect& inViewport)
    : Camera::Camera(inViewport)
    {
    }

    lost::math::Matrix& Camera2D::matrix()
    {
      if (needsUpdate)
      {
        m_matrix = math::MatrixProjection(math::Rect(-0.5, -0.5, m_viewport.width - m_viewport.x - 0.5, m_viewport.height - m_viewport.y - 0.5), math::Vec2(-1.0, 1.0));
        needsUpdate = false;
      }
      return m_matrix;
    }

  }
}
