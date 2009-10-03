#ifndef LOST_GL_TEXTURETILE_H
#define LOST_GL_TEXTURETILE_H

#include "lost/math/Rect.h"
#include "lost/gl/Texture.h"

namespace lost
{
namespace gl
{

/** A TextureTile defines a rectangular (sub)section of a texture in pixel coordinates.
 * The actual conversion to normalised texture coordinates can be performed with 
 * the helper functions of the texture.
 */
struct TextureTile
{
  TextureTile(){};
  TextureTile(const math::Rect& rect, const TexturePtr& texptr)
  : pixelCoords(rect), texture(texptr) {}
  
  math::Rect pixelCoords;
  TexturePtr texture;
};
}
}


#endif