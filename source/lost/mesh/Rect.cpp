/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/mesh/Rect.h"
#include "lost/gl/gl.h"
#include "lost/gl/HybridIndexBuffer.h"
#include "lost/gl/HybridVertexBuffer.h"

namespace lost
{
namespace mesh
{

Rectangle::Rectangle(const math::Rect& rect)
{
  gl::BufferLayout layout;
  layout.add(gl::ET_vec2_f32, gl::UT_position);
  this->resetBuffers(layout, gl::ET_u16);      

  indexBuffer->drawMode = GL_LINE_LOOP;
  uint32_t numVertices = 4;
  uint32_t numIndices = numVertices;
  
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

