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
  _discTextureDiameter = 0;
  _discTexture.reset(new Texture);
  maxDiameter = 256;
  _radiusOffset = -.5;
  _centerOffset = -.75f;
}

TextureManager::~TextureManager()
{
}

TexturePtr TextureManager::ringTexture(float diameter, float lineWidth)
{
  TexturePtr result;
  
  logStats();
  return result;
}

void TextureManager::updateTexture(gl::TexturePtr& tex, bool filled, float diameter)
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
    float cx = (diameter / 2.0f) + _centerOffset;
    float cy = (diameter / 2.0f) + _centerOffset;
    float cr = (diameter / 2.0f) + _radiusOffset;
    bmp->disc(cx, cy, cr);
    bitmaps.push_back(bmp);
    diameter = floor(diameter / 2.0f);
    ++numTextures;
  }
  DOUT("rebuild "<<numTextures<<" bitmaps for disc");

  tex->init(bitmaps, params); // preserves texture object, but reinitialises data  
}


void TextureManager::updateDiscTexture(float diameter)
{
  updateTexture(_discTexture, true, diameter);
}

// calculate maximum possible diameter for a disc texture given the following restrictions
// * must be power-of-two
// * must not be larger than maxDiameter
float TextureManager::calculateMaxDiameter(float diameter)
{
  float diameterp2 = (float)math::nextPowerOf2((uint32_t)diameter); // we need this anyway, new textures must be power of two
  float mdp2 = (float)math::nextPowerOf2((uint32_t)maxDiameter); // just to make sure the maximum is also always power of two
  float md = std::min(diameterp2, mdp2); // we need to recreate the texture to a certain maximum even if the user specified some ridiculously high value 
  return md;
}

// returns new safe radius if the texture needs to be updated, zero if it has to stay the same 
float TextureManager::textureNeedsUpdate(float requestedDiameter)
{
  float result= 0.0f;
  if(requestedDiameter > _discTextureDiameter) // only recalculate texture if incoming desired is larger than current diameter
  {
    float md = calculateMaxDiameter(requestedDiameter); // get safe maximum given the restrictions
    if(md > (_discTextureDiameter)) // and only build if the same maximum is larger than the current
    {
      result = md;
    }
  }
  return result;
}

gl::TexturePtr TextureManager::discTexture(float diameter)
{
  float newRadius;
  if(newRadius = textureNeedsUpdate(diameter))
  {
      updateDiscTexture(newRadius); 
      _discTextureDiameter = newRadius; 
  }
  
  logStats();
  return _discTexture;
}

void TextureManager::logStats()
{
  // FIXME: log texture count and approximate memory usage
}


}
}

