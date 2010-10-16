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
      layout.add(gl::ET_vec2_f32, gl::UT_position, 0);
      layout.add(gl::ET_vec2_f32, gl::UT_texcoord0, 0);
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
