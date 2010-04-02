#include "lost/mesh/RoundedRect.h"
#include "lost/mesh/TextureManager.h"
#include "lost/gl/gl.h"
#include "lost/gl/HybridIndexBuffer.h"
#include "lost/gl/HybridVertexBuffer.h"

namespace lost
{
namespace mesh
{

using namespace lost::math;
using namespace lost::gl;
using namespace std;

RoundedRect::RoundedRect(const TextureManagerPtr& tm, 
                         const math::Vec2 sz, 
                         bool f, 
                         float r, 
                         float lw) 
{
  textureManager = tm;
  filled = f;
  lineWidth = lw;
  tlr = r;
  trr = r;
  blr = r;
  brr = r;
  size = sz;
  commonInit();
  updateTexture(); 
  updateSize(size);
  createIndices();
  createTexcoords0();
  createTexcoords1();
}

RoundedRect::RoundedRect(const TextureManagerPtr& tm, 
                         const math::Vec2 sz, 
                         bool f, 
                         float intlr,
                         float intrr,
                         float inblr,
                         float inbrr,
                         float lw)
{
  textureManager = tm;
  filled = f;
  lineWidth = lw;
  tlr = intlr;
  trr = intrr;
  blr = inblr;
  brr = inbrr;
  size = sz;
  commonInit(); 
  updateTexture(); 
  updateSize(size);
  createIndices();
  createTexcoords0();
  createTexcoords1();
}

void RoundedRect::commonInit()
{
  gl::BufferLayout layout;
  layout.add(gl::ET_vec2_f32, gl::UT_position, 0);
  layout.add(gl::ET_vec2_f32, gl::UT_texcoord0, 0);
  layout.add(gl::ET_vec2_f32, gl::UT_texcoord1, 0);
  resetBuffers(layout, gl::ET_u16);  

  drawMode = GL_TRIANGLES;

  numVertices = 16; // draw it on paper and you'll see it's correct
  uint32_t numQuads = 9; // it's a 3x3 matrix of quads
  uint32_t numTris = numQuads*2; // each quad is drawn with two tris
  numIndices = numTris*3; // currently, each tri is drawn with 3 indices
  numTexCoords = numVertices; // each vertex receives one texcoord

  vertexBuffer->reset(numVertices);
  indexBuffer->reset(numIndices);
}

void RoundedRect::updateTexture()
{
  // calculate max diameter for request to textureManager
  float maxDiameter = std::max(tlr*2, std::max(trr*2, std::max(blr*2, brr*2)));
  TexturePtr tex;
  if(filled)
    tex = textureManager->discTexture(maxDiameter);
  else
    tex = textureManager->ringTexture(maxDiameter, lineWidth);
  
  if(material->textures.size() > 0)
    material->textures[0] = tex;
  else
    material->textures.push_back(tex);
}
  // vertices are created in this order:
  // 0  1  2  3
  // 4  5  6  7
  // 8  9  10 11
  // 12 13 14 15
  // 
  // 0,3,12,15 are the corners of the provided rect
void RoundedRect::updateSize(const Vec2& newSize)
{
  size = newSize;
}
  
void RoundedRect::createIndices()
{
  setIndex(0, 0);
  setIndex(1, 4);
  setIndex(2, 1);
  setIndex(3, 4);
  setIndex(4, 5);
  setIndex(5, 1);
  setIndex(6, 1);
  setIndex(7, 5);
  setIndex(8, 2);
  setIndex(9, 5);
  setIndex(10, 6);
  setIndex(11, 2);
  setIndex(12, 2);
  setIndex(13, 6);
  setIndex(14, 3);
  setIndex(15, 6);
  setIndex(16, 7);
  setIndex(17, 3);
  setIndex(18, 4);
  setIndex(19, 8);
  setIndex(20, 5);
  setIndex(21, 8);
  setIndex(22, 9);
  setIndex(23, 5);
  setIndex(24, 5);
  setIndex(25, 9);
  setIndex(26, 6);
  setIndex(27, 9);
  setIndex(28, 10);
  setIndex(29, 6);
  setIndex(30, 6);
  setIndex(31, 10);
  setIndex(32, 7);
  setIndex(33, 10);
  setIndex(34, 11);
  setIndex(35, 7);
  setIndex(36, 8);
  setIndex(37, 12);
  setIndex(38, 9);
  setIndex(39, 12);
  setIndex(40, 13);
  setIndex(41, 9);
  setIndex(42, 9);
  setIndex(43, 13);
  setIndex(44, 10);
  setIndex(45, 13);
  setIndex(46, 14);
  setIndex(47, 10);
  setIndex(48, 10);
  setIndex(49, 14);
  setIndex(50, 11);
  setIndex(51, 14);
  setIndex(52, 15);
  setIndex(53, 11);
}

void RoundedRect::createTexcoords0()
{
/*  setTexCoord(0, tex->normalisedCoord(math::Vec2(0,0)));
  setTexCoord(1, tex->normalisedCoord(math::Vec2(left, 0)));
  setTexCoord(2, tex->normalisedCoord(math::Vec2(tw-right, 0)));
  setTexCoord(3, tex->normalisedCoord(math::Vec2(tw, 0)));

  setTexCoord(4, tex->normalisedCoord(math::Vec2(0,bottom)));
  setTexCoord(5, tex->normalisedCoord(math::Vec2(left,bottom)));
  setTexCoord(6, tex->normalisedCoord(math::Vec2(tw-right,bottom)));
  setTexCoord(7, tex->normalisedCoord(math::Vec2(tw,bottom)));

  setTexCoord(8, tex->normalisedCoord(math::Vec2(0,th-top)));
  setTexCoord(9, tex->normalisedCoord(math::Vec2(left,th-top)));
  setTexCoord(10, tex->normalisedCoord(math::Vec2(tw-right, th-top)));
  setTexCoord(11, tex->normalisedCoord(math::Vec2(tw, th-top)));

  setTexCoord(12, tex->normalisedCoord(math::Vec2(0,th)));
  setTexCoord(13, tex->normalisedCoord(math::Vec2(left,th)));
  setTexCoord(14, tex->normalisedCoord(math::Vec2(tw-right,th)));
  setTexCoord(15, tex->normalisedCoord(math::Vec2(tw, th)));*/
}

void RoundedRect::createTexcoords1()
{
}

}
}