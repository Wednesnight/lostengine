/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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
