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
}

TextureManager::~TextureManager()
{
}

/*void TextureManager::updateArcFilledTexture(float radius)
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
    bmp->disc(0, 0, radius);
    bitmaps.push_back(bmp);
    radius = floor(radius / 2.0f);
    ++numTextures;
  }
  DOUT("rebuilt "<<numTextures<<" bitmaps for arcFilled");

  _arcFilledTexture->init(bitmaps, params); // preserves texture object, but reinitialises data  
}*/

TexturePtr TextureManager::createArcTexture(float lineWidth, float radius)
{
  TexturePtr result;
  
  BitmapPtr bmp = Bitmap::create(radius, radius, COMPONENTS_RGBA);
  bmp->ring(0, 0, radius, lineWidth);

  Texture::Params params;
  params.sizeHint = Texture::SIZE_POWER_OF_TWO;
  params.minFilter = GL_NEAREST;
  params.magFilter = GL_NEAREST;
  
  result.reset(new Texture(bmp, params));
  
  return result;
}

TexturePtr TextureManager::createArcFilledTexture(float radius)
{
  TexturePtr result;
  
  BitmapPtr bmp = Bitmap::create(radius, radius, COMPONENTS_RGBA);
  bmp->disc(0, 0, radius);

  Texture::Params params;
  params.sizeHint = Texture::SIZE_POWER_OF_TWO;
  params.minFilter = GL_NEAREST;
  params.magFilter = GL_NEAREST;
  
  result.reset(new Texture(bmp, params));
  
  return result;
}

gl::TexturePtr TextureManager::arcFilledTexture(float radius)
{
  TexturePtr result;
  ArcFilledMap::iterator pos = _arcFilledMap.find(radius);
  if(pos != _arcFilledMap.end())
  {
    result = pos->second;
  }
  else
  {
    result = createArcFilledTexture(radius);
    _arcFilledMap[radius] = result;
  }
  
  logStats();
  collectGarbage();
  return result;
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

