#include "lost/mesh/Disc.h"
#include "lost/gl/gl.h"
#include "lost/gl/HybridIndexBuffer.h"
#include "lost/gl/HybridVertexBuffer.h"
#include "lost/mesh/TextureManager.h"

namespace lost
{
namespace mesh
{

/**  
 *
 *
 */
Disc::Disc(const TextureManagerPtr& inTextureManager, bool f, float r, float lw)
{
  filled = f;
  radius = r;
  lineWidth = lw;
  textureManager = inTextureManager;

  gl::BufferLayout layout;
  layout.add(gl::ET_vec2_f32, gl::UT_position, 0);
  layout.add(gl::ET_vec2_f32, gl::UT_texcoord0, 0);
  this->resetBuffers(layout, gl::ET_u16);

  this->drawMode = GL_TRIANGLES;
  uint32_t numQuads = 1;
  uint32_t numVertices = numQuads*4;
  uint32_t numIndices = numQuads*6;

  this->vertexBuffer->reset(numVertices);
  this->indexBuffer->reset(numIndices);  
  
  createIndices();
  createTexCoords();
  update(filled, radius, lineWidth);
}

void Disc::createIndices()
{
  indexBuffer->set(0, gl::UT_index, (uint16_t)0);
  indexBuffer->set(1, gl::UT_index, (uint16_t)1);
  indexBuffer->set(2, gl::UT_index, (uint16_t)2);
  indexBuffer->set(3, gl::UT_index, (uint16_t)2);
  indexBuffer->set(4, gl::UT_index, (uint16_t)3);
  indexBuffer->set(5, gl::UT_index, (uint16_t)0);    
}

void Disc::createTexCoords()
{
  vertexBuffer->set(0, gl::UT_texcoord0, math::Vec2(0.0f,0.0f));
  vertexBuffer->set(1, gl::UT_texcoord0, math::Vec2(1.0f,0.0f));
  vertexBuffer->set(2, gl::UT_texcoord0, math::Vec2(1.0f,1.0f));
  vertexBuffer->set(3, gl::UT_texcoord0, math::Vec2(0.0f,1.0f));
}

void Disc::update(bool f, float r, float lw)
{
  filled = f;
  radius = r;
  lineWidth = lw;

  float diameter = radius * 2.0f;
  vertexBuffer->set(0, gl::UT_position, math::Vec2(0,0));
  vertexBuffer->set(1, gl::UT_position, math::Vec2(diameter,0));
  vertexBuffer->set(2, gl::UT_position, math::Vec2(diameter,diameter));
  vertexBuffer->set(3, gl::UT_position, math::Vec2(0,diameter));
  updateTexture(filled, radius, lineWidth);
}

void Disc::updateLineWidth(float newLineWidth)
{
  lineWidth = newLineWidth;
  update(filled, radius, lineWidth);
}

void Disc::updateRadius(float newRadius)
{
  radius = newRadius;
  update(filled, radius, lineWidth);
}

void Disc::updateTexture(bool filled, float radius, float lineWidth)
{
  gl::TexturePtr tex = textureManager->arcFilledTexture(radius);
/*  if(filled)
  {
    tex = textureManager->discTexture(diameter);
  }
  else 
  {
    tex = textureManager->ringTexture(diameter, lineWidth);
  }*/

  if(material->textures.size() > 0)
  {
    material->textures[0] = tex;
  }
  else
  {
    material->textures.push_back(tex);
  }
  
  
}

}
}
