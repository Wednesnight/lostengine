#ifndef LOST_MESH_RECT_H
#define LOST_MESH_RECT_H

#include "lost/mesh/Mesh.h"
#include "lost/math/Rect.h"

namespace lost
{
namespace mesh
{

  struct Rectangle;
  typedef Rectangle Rect;
  typedef lost::shared_ptr<Rect> RectPtr;

  struct Rectangle : public Mesh
  {
    Rectangle(const math::Rect& rect);

    void createIndices();
    void createVertices(const math::Rect& r);
    void updateSize(const math::Vec2& newSize);

    static RectPtr create(const math::Rect& rect)
    {
      return RectPtr(new Rect(rect));
    }

  };

}
}

#endif
