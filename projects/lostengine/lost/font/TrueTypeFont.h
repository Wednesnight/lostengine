#ifndef LOST_FONT_TRUETYPEFONT_H
#define LOST_FONT_TRUETYPEFONT_H

#include "lost/font/freetype/forward.h"
#include "lost/font/forward.h"
#include "lost/common/forward.h"
#include "lost/math/Rect.h"
#include "lost/font/Font.h"

namespace lost
{
namespace font
{

struct TrueTypeFont : Font
{  
  TrueTypeFont(freetype::LibraryPtr inLibrary,
               common::DataPtr inData,
               uint32_t inSizeInPoints);
  virtual ~TrueTypeFont();
  
  GlyphPtr glyph(uint32_t utf32character);
  bool hasKerning();
  float kerningOffset(uint32_t previousChar, uint32_t currentChar);
  float characterAdvance(uint32_t previousChar, uint32_t currentChar); 
  

private:
  freetype::FacePtr face;
  map<lost::text::utf32_char, GlyphPtr> char2glyph; 
  vector<GlyphPtr> glyphs; // this list of glyphs contains the glyphs in the order they were rendered
                                // this is important to preserve the ordering fo rth packing of the atlas


  /** checks if the caches already contain the glyph for the given character 
   *  and updates it if they don't.
   *  @return true if the glyph was rendered, false if it was cached and didn't need to be rendered again.
   */
  bool renderGlyph(lost::text::utf32_char c);
  void rebuildTextureAtlas(); // builds a new texture atlas from the current glyphs in the cache.
};
}  
}

#endif
