#ifndef LOST_GL_TEXTUREATALAS_H
#define LOST_GL_TEXTUREATALAS_H

#include "lost/gl/Texture.h"
#include "lost/math/Rect.h"
#include "lost/gl/TextureTile.h"
#include "lost/gl/TextureAtlas.h"

namespace lost
{
namespace gl
{

/** Maintains a texture and rectangular pixel sections defined within that texture.
 * The tiles are indexed with arbitrary but unique ids.
 */
struct TextureAtlas
{  
  TexturePtr texture;
  map<uint32_t, math::Rect> pixelCoordRects;
  
  TextureTile tile(uint32_t tileId)
  {
    return TextureTile(pixelCoordRects[tileId], texture);
  }
  
  static TextureAtlasPtr create() { return TextureAtlasPtr(new TextureAtlas); };
  
};
}
}

#endif