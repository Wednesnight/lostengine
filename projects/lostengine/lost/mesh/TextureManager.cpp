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
  _arcFilledRadius = 0;
  _arcFilledTexture.reset(new Texture);
  maxRadius = 256;
  _centerOffset = -.5f;
}

TextureManager::~TextureManager()
{
}

void TextureManager::updateArcFilledTexture(float radius)
{
  vector<BitmapPtr> bitmaps; // 0 = mimap level 0 = largest, all others are the following reduction levels

  Texture::Params params;
  params.sizeHint = Texture::SIZE_POWER_OF_TWO;
  params.minFilter = GL_LINEAR_MIPMAP_NEAREST;
  params.magFilter = GL_LINEAR;
  
  // create all mipmap levels
  uint32_t numTextures = 0;
  while(radius > 0)
  {
    BitmapPtr bmp(new Bitmap(radius, radius, bitmap::COMPONENTS_RGBA));
    bmp->disc(_centerOffset, _centerOffset, radius);
    bitmaps.push_back(bmp);
    radius = floor(radius / 2.0f);
    ++numTextures;
  }
  DOUT("rebuilt "<<numTextures<<" bitmaps for arcFilled");

  _arcFilledTexture->init(bitmaps, params); // preserves texture object, but reinitialises data  
}

TexturePtr TextureManager::createArcTexture(float lineWidth, float radius)
{
  TexturePtr result;
  
  BitmapPtr bmp = Bitmap::create(radius, radius, COMPONENTS_RGBA);
  bmp->ring(_centerOffset, _centerOffset, radius, lineWidth);

  Texture::Params params;
  params.sizeHint = Texture::SIZE_POWER_OF_TWO;
  params.minFilter = GL_LINEAR;
  params.magFilter = GL_LINEAR;
  
  result.reset(new Texture(bmp, params));
  
  return result;
}

// calculate maximum possible radius for a disc texture given the following restrictions
// * must be power-of-two
// * must not be larger than maxRadius
float TextureManager::calculateMaxRadius(float radius)
{
  float radiusp2 = (float)math::nextPowerOf2((uint32_t)radius); // we need this anyway, new textures must be power of two
  float mrp2 = (float)math::nextPowerOf2((uint32_t)maxRadius); // just to make sure the maximum is also always power of two
  float mr = std::min(radiusp2, mrp2); // we need to recreate the texture to a certain maximum even if the user specified some ridiculously high value 
  return mr;
}

// returns new safe radius if the texture needs to be updated, zero if it has to stay the same 
float TextureManager::textureNeedsUpdate(float requestedRadius)
{
  float result= 0.0f;  
  if(requestedRadius > _arcFilledRadius) // only recalculate texture if incoming desired is larger than current diameter
  {
    float mr = calculateMaxRadius(requestedRadius); // get safe maximum given the restrictions
    if(mr > (_arcFilledRadius)) // and only build if the same maximum is larger than the current
    {
      result = mr;
    }
  }
  return result;
}

gl::TexturePtr TextureManager::arcFilledTexture(float radius)
{
  float newRadius;
  if(newRadius = textureNeedsUpdate(radius))
  {
      updateArcFilledTexture(newRadius); 
      _arcFilledRadius = newRadius; 
  }
  
  logStats();
  collectGarbage();
  return _arcFilledTexture;
}

gl::TexturePtr TextureManager::arcTexture(float radius, float lineWidth)
{
  TexturePtr result;
  LineWidthRadius lwr = make_pair(lineWidth, radius);
  ArcMap::iterator pos = _arcMap.find(lwr);
  if(pos != _arcMap.end())
  {
    result = pos->second;
  }
  else
  {
    result = createArcTexture(lineWidth, radius);
    _arcMap[lwr] = result;
  }
  
  logStats();
  collectGarbage();
  return result;
}

void TextureManager::logStats()
{
  // FIXME: log texture count and approximate memory usage
}

void TextureManager::collectGarbage()
{
  // FIXME: throw  away all textures with use_count 1 (only referenced by texturemanager)
}

}
}

