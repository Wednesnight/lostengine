#ifndef LOST_GL_TEXTUREATALAS_H
#define LOST_GL_TEXTUREATALAS_H

#include "lost/gl/Texture.h"
#include <vector>
#include "lost/math/Rect.h"

namespace lost
{
namespace gl
{

struct TextureAtlas;
typedef lost::shared_ptr<TextureAtlas> TextureAtlasPtr;

struct TextureAtlas
{
  TextureAtlas()
  {
  }
  
  virtual ~TextureAtlas() {}
  
  TexturePtr tex;
  std::vector<math::Rect> pixelCoordRects;
  
  static TextureAtlasPtr create() { return TextureAtlasPtr(new TextureAtlas); };
  
};
}
}

#endif