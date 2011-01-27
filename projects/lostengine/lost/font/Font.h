#ifndef LOST_FONT_FONT_H
#define LOST_FONT_FONT_H

#include "lost/font/forward.h"
#include "lost/gl/forward.h"
#include "lost/math/Vec2.h"

namespace lost
{
namespace font
{
struct Font
{
  Font();
  virtual ~Font();
  
  virtual bool hasKerning() = 0;
  virtual GlyphPtr glyph(uint32_t utf32character) = 0;
  virtual float kerningOffset(uint32_t previousChar, uint32_t currentChar) = 0;
  virtual float characterAdvance(uint32_t previousChar, uint32_t currentChar) = 0; // returns advance (if glyph available)+kerningOffset (if kerning available), 0 otherwise. Use 0 if there is no prevchar

  uint32_t          size;
  gl::TexturePtr    atlas;  
  lost::math::Vec2  atlasSize;  
  float             lineHeight;
  float             ascender;
  float             descender;
};
}
}

#endif