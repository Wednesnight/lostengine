#ifndef LOST_MESH_LINE_H
#define LOST_MESH_LINE_H

#include "lost/mesh/Mesh.h"
#include "lost/mesh/forward.h"
#include "lost/math/forward.h"

namespace lost
{
  namespace mesh
  {

    struct Line;
    typedef lost::shared_ptr<Line> LinePtr;
    
    struct Line : public Mesh
    {
    private:
      TextureManagerPtr textureManager;
      math::Vec2 start, finish;
      math::Vec2 vector;
      float length;
      float angle;
      unsigned int width;

      Line(const TextureManagerPtr& inTextureManager, const math::Vec2& lineStart, const math::Vec2& lineFinish, const int lineWidth);
      
      void createIndices();
      void createVertices(bool flip);

    public:
      static LinePtr create(const TextureManagerPtr& inTextureManager, const math::Vec2& lineStart, const math::Vec2& lineFinish, const int lineWidth)
      {
        return LinePtr(new Line(inTextureManager, lineStart, lineFinish, lineWidth));
      }

      void update(const math::Vec2& lineStart, const math::Vec2& lineFinish, const int lineWidth);
        
    };
    
  }
}

#endif
