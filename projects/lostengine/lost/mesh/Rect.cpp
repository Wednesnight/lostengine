#include "lost/mesh/Rect.h"

namespace lost
{
namespace mesh
{

Rectangle::Rectangle(const math::Rect& rect)
{
  gl::BufferLayout layout;
  layout.add(gl::ET_vec2_f32, gl::UT_vertex, 0);
  this->resetBuffers(layout, gl::ET_u32);      

  this->drawMode = GL_LINE_LOOP;
  boost::uint32_t numVertices = 4;
  boost::uint32_t numIndices = numVertices;
  
  this->vertexBuffer->reset(numVertices);
  this->indexBuffer->reset(numIndices);
  
  this->createVertices(rect);
  this->createIndices();
  this->transform = math::MatrixTranslation(math::Vec3(rect.x, rect.y, 0));
}

void Rectangle::createIndices()
{
  this->setIndex(0,0);
  this->setIndex(1,1);
  this->setIndex(2,2);
  this->setIndex(3,3);
}

void Rectangle::createVertices(const math::Rect& r)
{
  math::Rect rect(0,0,r.width, r.height);        
  this->setVertex(0, rect.bottomLeft());
  this->setVertex(1, rect.bottomRight());
  this->setVertex(2, rect.topRight());
  this->setVertex(3, rect.topLeft());        
}

void Rectangle::updateSize(const math::Vec2& newSize)
{
  this->createVertices(math::Rect(0,0,newSize.x, newSize.y));
}

 }
}

