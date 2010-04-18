#include "lost/mesh/RoundedRect.h"
#include "lost/mesh/TextureManager.h"
#include "lost/gl/gl.h"
#include "lost/gl/HybridIndexBuffer.h"
#include "lost/gl/HybridVertexBuffer.h"
#include "lost/gl/Texture.h"

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
  radius = r;
  _size = sz;
  commonInit();
  updateTexture(); 
  updateVertices();
  updateIndices();
  updateTexcoords0();
  updateTexcoords1();
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

  roundBL = true;
  roundBR = true;
  roundTL = true;
  roundTR = true;
}

void RoundedRect::updateTexture()
{
  TexturePtr tex;
  if(filled)
    tex = textureManager->arcFilledTexture(radius);
  else
    tex = textureManager->arcTexture(radius, lineWidth);
  
  if(material->textures.size() > 0)
    material->textures[0] = tex;
  else
    material->textures.push_back(tex);
  textureManager->collectGarbage();
}

void RoundedRect::size(const Vec2& newSize)
{
  _size = newSize;
  updateVertices();
}

void RoundedRect::roundCorners(bool rbl, bool rbr, bool rtl, bool rtr)
{
  roundBL = rbl;
  roundBR = rbr;
  roundTL = rtl;
  roundTR = rtr;
  updateVertices();
  updateTexcoords1();
  collapseCorners();
}

// vertices are created in this order:
// 0  1  2  3
// 4  5  6  7
// 8  9  10 11
// 12 13 14 15
void RoundedRect::updateVertices()
{
  float top = _size.y;
  float top2 = top - radius;
  float bot = 0;
  float bot2 = bot + radius;
  float left = 0;
  float left2 = left+radius;
  float right = _size.x;
  float right2 = right - radius;
  
  vertexBuffer->set(0, UT_position, Vec2(left,top));
  vertexBuffer->set(1, UT_position, Vec2(left2,top));
  vertexBuffer->set(2, UT_position, Vec2(right2,top));
  vertexBuffer->set(3, UT_position, Vec2(right,top));

  vertexBuffer->set(4, UT_position, Vec2(left,top2));
  vertexBuffer->set(5, UT_position, Vec2(left2,top2));
  vertexBuffer->set(6, UT_position, Vec2(right2,top2));
  vertexBuffer->set(7, UT_position, Vec2(right,top2));

  vertexBuffer->set(8, UT_position, Vec2(left,bot2));
  vertexBuffer->set(9, UT_position, Vec2(left2,bot2));
  vertexBuffer->set(10, UT_position, Vec2(right2,bot2));
  vertexBuffer->set(11, UT_position, Vec2(right,bot2));

  vertexBuffer->set(12, UT_position, Vec2(left,bot));
  vertexBuffer->set(13, UT_position, Vec2(left2,bot));
  vertexBuffer->set(14, UT_position, Vec2(right2,bot));
  vertexBuffer->set(15, UT_position, Vec2(right,bot));
  
  updateTexcoords1();
  collapseCorners();  
}

void RoundedRect::collapseCorners()
{
  if(!roundBL)
  {
    vertexBuffer->set(8, UT_position, Vec2(0,0));
    vertexBuffer->set(13, UT_position, Vec2(0,0));
    
    vertexBuffer->set(8,UT_texcoord1, Vec2(0,0));    
    vertexBuffer->set(13,UT_texcoord1, Vec2(0,0));    
  }

  if(!roundBR)
  {
    vertexBuffer->set(11, UT_position, Vec2(_size.x,0));
    vertexBuffer->set(14, UT_position, Vec2(_size.x,0));

    vertexBuffer->set(11,UT_texcoord1, Vec2(1,0));    
    vertexBuffer->set(14,UT_texcoord1, Vec2(1,0));    
  }

  if(!roundTL)
  {
    vertexBuffer->set(1, UT_position, Vec2(0,_size.y));
    vertexBuffer->set(4, UT_position, Vec2(0,_size.y));

    vertexBuffer->set(1,UT_texcoord1, Vec2(0,1));    
    vertexBuffer->set(4,UT_texcoord1, Vec2(0,1));    
  }

  if(!roundTR)
  {
    vertexBuffer->set(2, UT_position, _size);
    vertexBuffer->set(7, UT_position, _size);

    vertexBuffer->set(2,UT_texcoord1, Vec2(1,1));    
    vertexBuffer->set(7,UT_texcoord1, Vec2(1,1));    
  }
}
  
void RoundedRect::updateIndices()
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

void RoundedRect::updateTexcoords0()
{
  Vec2 maxtc = material->textures[0]->topRightTexCoord();
  float mx = maxtc.x;
  float my = maxtc.y;

  vertexBuffer->set(0,UT_texcoord0, Vec2(mx,my));
  vertexBuffer->set(1,UT_texcoord0, Vec2(0,my));
  vertexBuffer->set(2,UT_texcoord0, Vec2(0,my));
  vertexBuffer->set(3,UT_texcoord0, Vec2(mx,my));

  vertexBuffer->set(4,UT_texcoord0, Vec2(mx,0));
  vertexBuffer->set(5,UT_texcoord0, Vec2(0,0));
  vertexBuffer->set(6,UT_texcoord0, Vec2(0,0));
  vertexBuffer->set(7,UT_texcoord0, Vec2(mx,0));

  vertexBuffer->set(8,UT_texcoord0, Vec2(mx,0));
  vertexBuffer->set(9,UT_texcoord0, Vec2(0,0));
  vertexBuffer->set(10,UT_texcoord0, Vec2(0,0));
  vertexBuffer->set(11,UT_texcoord0, Vec2(mx,0));

  vertexBuffer->set(12,UT_texcoord0, Vec2(mx,my));
  vertexBuffer->set(13,UT_texcoord0, Vec2(0,my));
  vertexBuffer->set(14,UT_texcoord0, Vec2(0,my));
  vertexBuffer->set(15,UT_texcoord0, Vec2(mx,my));
}

void RoundedRect::updateTexcoords1()
{
  float bot = radius/_size.y;
  float top = (_size.y-radius)/_size.y;
  float left = radius/_size.x;
  float right = (_size.x-radius)/_size.x;

  vertexBuffer->set(0,UT_texcoord1, Vec2(0,1.0f));
  vertexBuffer->set(1,UT_texcoord1, Vec2(left,1.0f));
  vertexBuffer->set(2,UT_texcoord1, Vec2(right,1.0f));
  vertexBuffer->set(3,UT_texcoord1, Vec2(1.0f,1.0f));

  vertexBuffer->set(4,UT_texcoord1, Vec2(0,top));
  vertexBuffer->set(5,UT_texcoord1, Vec2(left,top));
  vertexBuffer->set(6,UT_texcoord1, Vec2(right,top));
  vertexBuffer->set(7,UT_texcoord1, Vec2(1.0f,top));

  vertexBuffer->set(8,UT_texcoord1, Vec2(0,bot));
  vertexBuffer->set(9,UT_texcoord1, Vec2(left,bot));
  vertexBuffer->set(10,UT_texcoord1, Vec2(right,bot));
  vertexBuffer->set(11,UT_texcoord1, Vec2(1.0f,bot));

  vertexBuffer->set(12,UT_texcoord1, Vec2(0,0.0f));
  vertexBuffer->set(13,UT_texcoord1, Vec2(left,0.0f));
  vertexBuffer->set(14,UT_texcoord1, Vec2(right,0.0f));
  vertexBuffer->set(15,UT_texcoord1, Vec2(1.0f,0.0f));  
}

}
}