#include "lost/mesh/Disc.h"
#include "lost/gl/gl.h"
#include "lost/gl/HybridIndexBuffer.h"
#include "lost/gl/HybridVertexBuffer.h"
#include "lost/mesh/TextureManager.h"
#include "lost/gl/Texture.h"

namespace lost
{
namespace mesh
{

using namespace lost::math;

/** consists of 4 quads that mirror an arc texture to create a filled disc/ring.
 * 9 Vertices are numbered like this:
 * 6 7 8
 * 3 4 5
 * 0 1 2
 *
 * Since the texture is an arc with its origin at bitmap 0,0, the primary texture coordinates are:
 * (1,1) (0,1) (1,1)
 * (1,0) (0,0) (1,0)
 * (1,1) (0,1) (1,1)
 * 
 * Secondary texture coordinates spread a [0,1] range across the whole mesh like this:
 * (0,1) (.5,1) (1,1)
 * (0,.5) (.5,.5) (1,.5)
 * (0,0) (.5,0) (1,0)
 */
Disc::Disc(const TextureManagerPtr& inTextureManager, bool f, float r, float lw)
{
  filled = f;
  radius = r;
  lineWidth = lw;
  textureManager = inTextureManager;

  gl::BufferLayout layout;
  layout.add(gl::ET_vec2_f32, gl::UT_position, 0);
  layout.add(gl::ET_vec2_f32, gl::UT_texcoord0, 0); // primary texcoords for arctexture
  layout.add(gl::ET_vec2_f32, gl::UT_texcoord1, 0); // secondary coords for texturemultiplication
  this->resetBuffers(layout, gl::ET_u16);

  this->drawMode = GL_TRIANGLES;
  uint32_t numQuads = 4;
  uint32_t numVertices = numQuads*4;
  uint32_t numIndices = numQuads*6;

  this->vertexBuffer->reset(numVertices);
  this->indexBuffer->reset(numIndices);  
  
  update(filled, radius, lineWidth); // must be called before updateTexCoords because texture is referenced later on
  updateIndices();
  updateTexCoords0();
  updateTexCoords1();
}

void Disc::updateIndices()
{
  indexBuffer->set(0, gl::UT_index, (uint16_t)0);
  indexBuffer->set(1, gl::UT_index, (uint16_t)1);
  indexBuffer->set(2, gl::UT_index, (uint16_t)4);
  indexBuffer->set(3, gl::UT_index, (uint16_t)4);
  indexBuffer->set(4, gl::UT_index, (uint16_t)3);
  indexBuffer->set(5, gl::UT_index, (uint16_t)0);    
  indexBuffer->set(6, gl::UT_index, (uint16_t)1);
  indexBuffer->set(7, gl::UT_index, (uint16_t)2);    
  indexBuffer->set(8, gl::UT_index, (uint16_t)5);
  indexBuffer->set(9, gl::UT_index, (uint16_t)5);    
  indexBuffer->set(10, gl::UT_index, (uint16_t)4);
  indexBuffer->set(11, gl::UT_index, (uint16_t)1);
  indexBuffer->set(12, gl::UT_index, (uint16_t)3);
  indexBuffer->set(13, gl::UT_index, (uint16_t)4);
  indexBuffer->set(14, gl::UT_index, (uint16_t)7);
  indexBuffer->set(15, gl::UT_index, (uint16_t)7);    
  indexBuffer->set(16, gl::UT_index, (uint16_t)6);
  indexBuffer->set(17, gl::UT_index, (uint16_t)3);    
  indexBuffer->set(18, gl::UT_index, (uint16_t)4);
  indexBuffer->set(19, gl::UT_index, (uint16_t)8);    
  indexBuffer->set(20, gl::UT_index, (uint16_t)7);
  indexBuffer->set(21, gl::UT_index, (uint16_t)4);    
  indexBuffer->set(22, gl::UT_index, (uint16_t)5);
  indexBuffer->set(23, gl::UT_index, (uint16_t)8);
}

void Disc::updateTexCoords0()
{
  Vec2 maxtc = material->textures[0]->topRightTexCoord();
  float mx = maxtc.x;
  float my = maxtc.y;

  vertexBuffer->set(0, gl::UT_texcoord0, Vec2(mx,my));
  vertexBuffer->set(1, gl::UT_texcoord0, Vec2(0.0f,my));
  vertexBuffer->set(2, gl::UT_texcoord0, Vec2(mx,my));

  vertexBuffer->set(3, gl::UT_texcoord0, Vec2(mx,0.0f));
  vertexBuffer->set(4, gl::UT_texcoord0, Vec2(0.0f,0.0f));
  vertexBuffer->set(5, gl::UT_texcoord0, Vec2(mx,0.0f));

  vertexBuffer->set(6, gl::UT_texcoord0, Vec2(mx,my));
  vertexBuffer->set(7, gl::UT_texcoord0, Vec2(0.0f,my));
  vertexBuffer->set(8, gl::UT_texcoord0, Vec2(mx,my));  
}

void Disc::updateTexCoords1()
{
  vertexBuffer->set(0, gl::UT_texcoord1, Vec2(0.0f,0.0f));
  vertexBuffer->set(1, gl::UT_texcoord1, Vec2(.5f,0.0f));
  vertexBuffer->set(2, gl::UT_texcoord1, Vec2(1.0f,0.0f));

  vertexBuffer->set(3, gl::UT_texcoord1, Vec2(0.0f,.5f));
  vertexBuffer->set(4, gl::UT_texcoord1, Vec2(.5f,.5f));
  vertexBuffer->set(5, gl::UT_texcoord1, Vec2(1.0f,.5f));

  vertexBuffer->set(6, gl::UT_texcoord1, Vec2(0.0f,1.0f));
  vertexBuffer->set(7, gl::UT_texcoord1, Vec2(.5f,1.0f));
  vertexBuffer->set(8, gl::UT_texcoord1, Vec2(1.0f,1.0f));
}

void Disc::updateVertices()
{
  float diameter = 2*radius;
  vertexBuffer->set(0, gl::UT_position, Vec2(0,0));
  vertexBuffer->set(1, gl::UT_position, Vec2(radius,0));
  vertexBuffer->set(2, gl::UT_position, Vec2(diameter,0));

  vertexBuffer->set(3, gl::UT_position, Vec2(0,radius));
  vertexBuffer->set(4, gl::UT_position, Vec2(radius,radius));
  vertexBuffer->set(5, gl::UT_position, Vec2(diameter,radius));

  vertexBuffer->set(6, gl::UT_position, Vec2(0,diameter));
  vertexBuffer->set(7, gl::UT_position, Vec2(radius,diameter));
  vertexBuffer->set(8, gl::UT_position, Vec2(diameter,diameter));
}

void Disc::update(bool f, float r, float lw)
{
  filled = f;
  radius = r;
  lineWidth = lw;
  updateVertices();
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
  gl::TexturePtr tex;
  if(filled)
  {
    tex = textureManager->arcFilledTexture(radius);
  }
  else 
  {
    tex = textureManager->arcTexture(radius, lineWidth);
  }

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
