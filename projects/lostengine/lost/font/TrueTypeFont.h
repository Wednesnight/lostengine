#ifndef LOST_FONT_TRUETYPEFONT_H
#define LOST_FONT_TRUETYPEFONT_H

#include <vector>
#include <string>
#include <map>

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
  
  void prepareGlyphs(const fhtagn::text::utf32_string& inText); // renders glyphs if missing from atlas, rebuilds atlas if required
  GlyphPtr glyph(uint32_t utf32character);
  bool hasKerning();
  float kerningOffset(uint32_t previousChar, uint32_t currentChar);
  

private:
  freetype::FacePtr face;
  std::map<fhtagn::text::utf32_char_t, GlyphPtr> char2glyph; 
  std::vector<GlyphPtr> glyphs; // this list of glyphs contains the glyphs in the order they were rendered
                                // this is important to preserve the ordering fo rth packing of the atlas


  /** checks if the caches already contain the glyph for the given character 
   *  and updates it if they don't.
   *  @return true if the glyph was rendered, false if it was cached and didn't need to be rendered again.
   */
  bool renderGlyph(fhtagn::text::utf32_char_t c);
  void rebuildTextureAtlas(); // builds a new texture atlas from the current glyphs in the cache.
  void flagDrawableChars(const fhtagn::text::utf32_string& inText);
};
}  
}

#endif
