#include "lost/math/Vec2.h"
#include "lost/gl/gl.h"
#include "lost/gl/HybridIndexBuffer.h"
#include "lost/gl/HybridVertexBuffer.h"

#include "lost/mesh/Line.h"

namespace lost
{
  namespace mesh
  {
    
    Line::Line(const math::Vec2& start, const math::Vec2& finish)
    {
      gl::BufferLayout layout;
      layout.add(gl::ET_vec2_f32, gl::UT_position, 0);
      this->resetBuffers(layout, gl::ET_u16);      
      
      indexBuffer->drawMode = GL_LINES;
      uint32_t numVertices = 2;
      uint32_t numIndices = numVertices;
      
      this->vertexBuffer->reset(numVertices);
      this->indexBuffer->reset(numIndices);
      
      this->createVertices(start, finish);
      this->createIndices();
    }
    
    void Line::createIndices()
    {
      this->setIndex(0,0);
      this->setIndex(1,1);
    }
    
    void Line::createVertices(const math::Vec2& start, const math::Vec2& finish)
    {
      this->setVertex(0, start);
      this->setVertex(1, finish);
    }
    
  }
}
