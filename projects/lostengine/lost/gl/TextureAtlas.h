#ifndef LOST_GL_TEXTUREATALAS_H
#define LOST_GL_TEXTUREATALAS_H

#include "lost/gl/Texture.h"
#include <map>
#include "lost/math/Rect.h"
#include "lost/gl/TextureTile.h"

namespace lost
{
namespace gl
{

struct TextureAtlas;
typedef lost::shared_ptr<TextureAtlas> TextureAtlasPtr;

/** Maintains a texture and rectangular pixel sections defined within that texture.
 * The tiles are indexed with arbitrary but unique ids.
 */
struct TextureAtlas
{  
  TexturePtr texture;
  std::map<uint32_t, math::Rect> pixelCoordRects;
  
  TextureTile tile(uint32_t tileId)
  {
    return TextureTile(pixelCoordRects[tileId], texture);
  }
  
  static TextureAtlasPtr create() { return TextureAtlasPtr(new TextureAtlas); };
  
};
}
}

#endif