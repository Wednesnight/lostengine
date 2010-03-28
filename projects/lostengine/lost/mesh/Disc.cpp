#include "lost/mesh/Disc.h"
#include "lost/gl/gl.h"
#include "lost/gl/HybridIndexBuffer.h"
#include "lost/gl/HybridVertexBuffer.h"
#include "lost/mesh/TextureManager.h"

namespace lost
{
namespace mesh
{

Disc::Disc(const TextureManagerPtr& inTextureManager, float diameter)
{
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
  updateSize(diameter);
  updateTexture(diameter);
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

void Disc::updateSize(float diameter)
{
  float radius = diameter / 2.0f;
  vertexBuffer->set(0, gl::UT_position, math::Vec2(-radius,-radius));
  vertexBuffer->set(1, gl::UT_position, math::Vec2(radius,-radius));
  vertexBuffer->set(2, gl::UT_position, math::Vec2(radius,radius));
  vertexBuffer->set(3, gl::UT_position, math::Vec2(-radius,radius));
  updateTexture(diameter);
}

void Disc::updateTexture(float diameter)
{
  if(material->textures.size() > 0)
  {
    material->textures[0] = textureManager->discTexture(diameter);
  }
  else
  {
    material->textures.push_back(textureManager->discTexture(diameter));
  }
}

}
}
