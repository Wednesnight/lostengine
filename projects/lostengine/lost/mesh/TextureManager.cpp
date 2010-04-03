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
  DOUT("arcFilled: "<<_arcFilledMap.size() << " arc:"<<_arcMap.size());
}

void TextureManager::collectGarbage()
{
    vector<LineWidthRadius> arcGarbage;
    vector<float> arcFilledGarbage;
    
    for(ArcFilledMap::iterator i=_arcFilledMap.begin(); i!=_arcFilledMap.end(); ++i)
    {
      if(i->second.use_count() == 1)
      {
        arcFilledGarbage.push_back(i->first);
      }
    }
    uint32_t afg = arcFilledGarbage.size();
    if(afg)
    {
      DOUT("arcFilled garbage: "<<arcFilledGarbage.size());
      for(uint32_t i=0; i<arcFilledGarbage.size(); ++i)
      {
        _arcFilledMap.erase(arcFilledGarbage[i]);
      }
    }
    
    for(ArcMap::iterator i=_arcMap.begin(); i!=_arcMap.end(); ++i)
    {
      if(i->second.use_count() == 1)
      {
        arcGarbage.push_back(i->first);
      }
    }
    uint32_t ag = arcGarbage.size();
    if(ag)
    {
      DOUT("arc garbage: "<<arcGarbage.size());
      for(uint32_t i=0; i<arcGarbage.size(); ++i)
      {
        _arcMap.erase(arcGarbage[i]);
      }
    }
    
}

}
}

