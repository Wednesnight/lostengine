#ifndef LOST_MESH_LINE_H
#define LOST_MESH_LINE_H

#include "lost/mesh/Mesh.h"
#include "lost/math/forward.h"

namespace lost
{
  namespace mesh
  {
    
    struct Line;
    typedef lost::shared_ptr<Line> LinePtr;
    
    struct Line : public Mesh
    {
      Line(const math::Vec2& start, const math::Vec2& finish);
      
      void createIndices();
      void createVertices(const math::Vec2& start, const math::Vec2& finish);

      static LinePtr create(const math::Vec2& start, const math::Vec2& finish)
      {
        return LinePtr(new Line(start, finish));
      }
      
    };
    
  }
}

#endif
