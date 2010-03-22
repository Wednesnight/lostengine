#include "lost/mesh/TextureManager.h"
#include "lost/math/lmath.h"
#include "lost/gl/Texture.h"
#include "lost/bitmap/Bitmap.h"
#include <vector>
#include "lost/gl/gl.h"

namespace lost
{
namespace mesh
{

using namespace lost::math;
using namespace lost::bitmap;
using namespace lost::gl;
using namespace std;

TextureManager::TextureManager()
{
  _discTextureRadius = 0;
  _discTexture.reset(new Texture);
}

TextureManager::~TextureManager()
{
}

TexturePtr TextureManager::ringTexture(float radius, float lineWidth)
{
  TexturePtr result;
  
  logStats();
  return result;
}

void TextureManager::updateDiscTexture(float diameter)
{
  vector<BitmapPtr> bitmaps; // 0 = mimap level 0 = largest, all others are the following reduction levels

  Texture::Params params;
  params.sizeHint = Texture::SIZE_POWER_OF_TWO;
  params.minFilter = GL_LINEAR_MIPMAP_LINEAR;
  params.magFilter = GL_LINEAR;
  
  // create all mipmap levels
  uint32_t numTextures = 0;
  while(diameter > 0)
  {
    BitmapPtr bmp(new Bitmap(diameter, diameter, bitmap::COMPONENTS_RGBA));
    float cx = (diameter / 2.0f) - .5f;
    float cy = cx;
    float cr = cy;
    bmp->disc(cx, cy, cr);
    bitmaps.push_back(bmp);
    diameter = floor(diameter / 2.0f);
    ++numTextures;
  }
  DOUT("rebuild "<<numTextures<<" bitmaps for disc");

  _discTexture->init(bitmaps, params); // preserves texture object, but reinitialises data
}

gl::TexturePtr TextureManager::discTexture(float radius)
{
  if(radius > _discTextureRadius)
  {
    float diameter = (float)math::nextPowerOf2((uint32_t)(radius*2));
    updateDiscTexture(diameter);
    _discTextureRadius = radius;
  }
  
  logStats();
  return _discTexture;
}

void TextureManager::logStats()
{
}


}
}