#include "lost/mesh/TextureManager.h"
#include "lost/math/lmath.h"
#include "lost/gl/Texture.h"
#include "lost/bitmap/Bitmap.h"
#include <vector>
#include "lost/gl/gl.h"
#include "lost/common/Color.h"
#include "lost/common/ColorGradient.h"
#include "lost/common/Logger.h"

namespace lost
{
namespace mesh
{

using namespace lost::math;
using namespace lost::bitmap;
using namespace lost::gl;
using namespace lost::common;
using namespace std;

TextureManager::TextureManager()
{
  _gradientTextureHeight = 256;
  gradientTextureWidth = 256;
  _gradientWidth = 3;
  _maxNumGradients = gradientTextureWidth / _gradientWidth;
  gradientTexture.reset(new Texture());
}

TextureManager::~TextureManager()
{
}

gl::TexturePtr TextureManager::createLineTexture(const unsigned int lineWidth) {

  TexturePtr result;

  BitmapPtr bmp = Bitmap::create(1, lineWidth, COMPONENTS_RGBA);
  int midNum = 2 - (lineWidth % 2);
  for (unsigned int idx = 0; idx < midNum; ++idx) {

    bmp->pixel(0, (((lineWidth + midNum) / 2) - 1) - idx, Color(1.0f, 1.0f, 1.0f, 1.0f));
  }

  for (unsigned int idx = 0; idx < ((lineWidth + midNum) / 2) - midNum; ++idx) {

    float alpha = (idx + 1) * (1.0f / ((lineWidth + midNum) / 2));
    Color col = Color(alpha, alpha, alpha, alpha);
    bmp->pixel(0, idx, col);
    bmp->pixel(0, (lineWidth - 1) - idx, col);
  }

  Texture::Params params;
//  params.sizeHint = Texture::SIZE_POWER_OF_TWO;
  params.minFilter = GL_LINEAR;
  params.magFilter = GL_LINEAR;

  result.reset(new Texture(bmp, params));
  
  return result;
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

gl::TexturePtr TextureManager::lineTexture(const unsigned int lineWidth)
{
  TexturePtr result;
  LineMap::iterator pos = _lineMap.find(lineWidth);
  if(pos != _lineMap.end()) {

    result = pos->second;
  } else {

    result = createLineTexture(lineWidth);
    _lineMap[lineWidth] = result;
  }
  
  logStats();
  collectGarbage();
  return result;
}
  
void TextureManager::logStats()
{
//  DOUT("arcFilled: "<<_arcFilledMap.size() << " arc:"<<_arcMap.size() << " line:"<<_lineMap.size());
}

void TextureManager::collectGarbage()
{
    vector<unsigned int> lineGarbage;
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

    for(LineMap::iterator i=_lineMap.begin(); i!=_lineMap.end(); ++i)
    {
      if(i->second.use_count() == 1)
      {
        lineGarbage.push_back(i->first);
      }
    }
    uint32_t lg = lineGarbage.size();
    if(lg)
    {
      DOUT("line garbage: "<<lineGarbage.size());
      for(uint32_t i=0; i<lineGarbage.size(); ++i)
      {
        _lineMap.erase(lineGarbage[i]);
      }
    }
}

float TextureManager::addGradient(const common::ColorGradientPtr& inGradient)
{
  float result;
  
  assert(_gradients.size() < _maxNumGradients);
  _gradients.push_back(inGradient);
  updateGradientTexture();

  float center = ceil(((float)_gradientWidth)/2.0f);
  result = _gradients.size()*_gradientWidth;
  result -= center;
  result /= (float)gradientTextureWidth;
  
  return result;
}



void TextureManager::updateGradientTexture()
{
  BitmapPtr bitmap = Bitmap::create(gradientTextureWidth, _gradientTextureHeight, COMPONENTS_RGBA);
  bitmap->clear(Color(0,0,0,0));
  Texture::Params params;
  params.minFilter = GL_NEAREST;
  params.magFilter = GL_NEAREST;
  
  float x = 0.0f;
  float gh = _gradientTextureHeight-1;
  for(uint32_t gidx=0; gidx<_gradients.size(); ++gidx)
  {
    for(float y=0.0f; y<_gradientTextureHeight; y+=1.0f)
    {
      float ccoord = y/gh;
      Color col = _gradients[gidx]->colorAt(ccoord);
      for(uint32_t ix=0; ix<_gradientWidth; ++ix)
      {
        bitmap->pixel(x+ix, y, col);
      }
    }
    x+=_gradientWidth;
  }
  
  gradientTexture->init(bitmap, params);
}


}
}

