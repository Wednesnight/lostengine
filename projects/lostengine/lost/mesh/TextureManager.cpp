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
  _centerOffset = -.5f;
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

gl::TexturePtr TextureManager::discTexture(float diameter)
{
  if(diameter > _discTextureDiameter)
  {
    float diameterp2 = (float)math::nextPowerOf2((uint32_t)diameter); // we need this anyway, new textures must be power of two
    float mdp2 = (float)math::nextPowerOf2((uint32_t)maxDiameter); // just to make sure the maximum is also always power of two
    float md = std::min(diameterp2, mdp2); // we need to recreate the texture to a certain maximum even if the user specified some ridiculously high value 
    if(md > (_discTextureDiameter)) // so check if the suggested diameter, clamped against mdp2, is larger than the current texture an recreate if necessary
    {
      updateDiscTexture(md); // use md here because diameter contains the desired value, but md contains the allowed maximum
      _discTextureDiameter = md; // memorize the processed md/2, since its power of two, instead of probably bogus input value
    }
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

