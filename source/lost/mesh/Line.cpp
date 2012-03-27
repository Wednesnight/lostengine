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

#include "lost/math/Vec2.h"
#include "lost/gl/gl.h"
#include "lost/gl/HybridIndexBuffer.h"
#include "lost/gl/HybridVertexBuffer.h"
#include "lost/gl/Texture.h"

#include "lost/mesh/Line.h"
#include "lost/bitmap/Bitmap.h"
#include "lost/math/Matrix.h"
#include "lost/math/lmath.h"
#include "lost/common/Logger.h"
#include "lost/common/Color.h"
#include "lost/mesh/TextureManager.h"

using namespace lost::bitmap;
using namespace lost::common;
using namespace lost::gl;
using namespace lost::math;

namespace lost
{
  namespace mesh
  {
    
    Line::Line(const TextureManagerPtr& inTextureManager, const math::Vec2& lineStart,
               const math::Vec2& lineFinish, const int lineWidth) {

      textureManager = inTextureManager;

      gl::BufferLayout layout;
      layout.add(gl::ET_vec2_f32, gl::UT_position);
      layout.add(gl::ET_vec2_f32, gl::UT_texcoord0);
      this->resetBuffers(layout, gl::ET_u16);      
      
      indexBuffer->drawMode = GL_TRIANGLE_STRIP;
      uint32_t numVertices = 4;
      uint32_t numIndices = 4;
      
      this->vertexBuffer->reset(numVertices);
      this->indexBuffer->reset(numIndices);

      this->createIndices();

      update(lineStart, lineFinish, lineWidth);
    }

    void Line::update(const math::Vec2& lineStart, const math::Vec2& lineFinish, const int lineWidth) {

      start = lineStart;
      finish = lineFinish;
      vector = finish - start;
      length = len(vector);
      angle = math::angle(Vec2(length, 0.0f), vector);
      width = lineWidth + 2;
      
      this->createVertices(start.x > finish.x);
      
      TexturePtr tex = textureManager->lineTexture(width);
      if (material->textures.size() > 0) {
        
        material->textures[0] = tex;
      } else {
        
        material->textures.push_back(tex);
      }
      
      this->transform = MatrixTranslation(Vec3(start.x, start.y, 0)) *
          MatrixRotZ((start.y > finish.y) ? angle : -angle);
    }

    void Line::createIndices()
    {
      this->setIndex(0,0);
      this->setIndex(1,1);
      this->setIndex(2,2);
      this->setIndex(3,3);
    }
    
    void Line::createVertices(bool flip)
    {
      float half = (float)width / 2.0f;
      this->setVertex(0, Vec2(0.0f, -half));
      this->setVertex(1, Vec2(0.0f, half));
      this->setVertex(2, Vec2(length, -half));
      this->setVertex(3, Vec2(length, half));

      this->vertexBuffer->set(0, UT_texcoord0, Vec2(0.0f, 0.0f));
      this->vertexBuffer->set(1, UT_texcoord0, Vec2(0.0f, 1.0f));
      this->vertexBuffer->set(2, UT_texcoord0, Vec2(1.0f, 0.0f));
      this->vertexBuffer->set(3, UT_texcoord0, Vec2(1.0f, 1.0f));
    }
    
  }
}
