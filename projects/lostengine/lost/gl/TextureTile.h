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
  // describes the orientation of the tile in the texture
  // this affects the generation of normalised texture coordinates.
  enum Orientation
  {
    ORIENT_NORMAL=0,
    ORIENT_CW90=1, // clockwise rotation, 90 degrees
    ORIENT_FLIPY=2   // mirrored about x axis, so top and bottom y coordinates are exchanged
  };
  
  TextureTile(){};
  TextureTile(const math::Rect& rect, const TexturePtr& texptr)
  : pixelCoords(rect), texture(texptr) {}
  
  void calculateTextureCoords(math::Vec2& botLeft,
                              math::Vec2& botRight,
                              math::Vec2& topLeft,
                              math::Vec2& topRight)
  {
    // create normalised texture coordinates for normal orientation
    botLeft = texture->normalisedCoord(pixelCoords.bottomLeft());

    math::Vec2 t = pixelCoords.bottomLeft();
    t.x += pixelCoords.width;
    t.y += pixelCoords.height;
    topRight = texture->normalisedCoord(t);

    botRight = botLeft;
    botRight.x = topRight.x;

    topLeft = botLeft;
    topLeft.y = topRight.y;
  
    math::Vec2 bl, br, tl, tr;

    // swap as needed for other orienatations
    switch (orientation) {
      case ORIENT_NORMAL:break;
      case ORIENT_FLIPY:        
        std::swap(botLeft, topLeft);
        std::swap(botRight, topLeft);
        break;
      case ORIENT_CW90: 
        bl = botLeft;
        br = botRight;
        tl = topLeft;
        tr = topRight;
        topLeft = bl;
        topRight = tl;
        botLeft = br;
        botRight = tr;
        break;
      default:break;
    }
  }
  
  math::Rect pixelCoords;
  TexturePtr texture;
  Orientation orientation;
};
}
}


#endif