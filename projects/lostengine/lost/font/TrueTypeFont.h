#ifndef LOST_FONT_TRUETYPEFONT_H
#define LOST_FONT_TRUETYPEFONT_H

#include <vector>
#include <string>
#include <map>

#include "lost/font/freetype/forward.h"
#include "lost/font/forward.h"
#include "lost/gl/forward.h"
#include "lost/bitmap/forward.h"
#include "lost/common/forward.h"

#include "lost/math/Vec2.h"
#include "lost/math/Rect.h"

namespace lost
{
namespace font
{

struct TrueTypeFont
{  
  TrueTypeFont(freetype::LibraryPtr inLibrary,
               common::DataPtr inData,
               uint32_t inSizeInPoints);
  virtual ~TrueTypeFont();
    
  // renders the given text, creating a new mesh in the process.  
  RenderedTextPtr render(const fhtagn::text::utf32_string& inText);
  RenderedTextPtr render(const std::string & inText);

  // same as above, but the provided mesh is reused 
  void render(const fhtagn::text::utf32_string& inText, const RenderedTextPtr& target);
  void render(const std::string & inText, const RenderedTextPtr& target);
  
  
  
  freetype::FacePtr face;
  lost::math::Vec2  atlasSize;
  gl::TexturePtr    atlas;
  std::map<fhtagn::text::utf32_char_t, GlyphPtr> char2glyph; 
  std::vector<GlyphPtr> glyphs; // this list of glyphs contains the glyphs in the order they were rendered
                                // this is important to preserve the ordering fo rth packing of the atlas
  float ascender;
  float descender;

  uint32_t size() { return _size; } // return size of the font in points 

private:

  /** checks if the caches already contain the glyph for the given character 
   *  and updates it if they don't.
   *  @return true if the glyph was rendered, false if it was cached and didn't need to be rendered again.
   */
  bool renderGlyph(fhtagn::text::utf32_char_t c);
  void rebuildTextureAtlas(); // builds a new texture atlas from the current glyphs in the cache.
  void flagDrawableChars(const fhtagn::text::utf32_string& inText);
  void addGlyph(std::vector<math::Rect>& characterRects, // receives a rect that describes character quad geometry 
                std::vector<math::Rect>& pixelCoordRects, // receives a rect that describes the character quads subtexture inside the font texture atlas
                GlyphPtr glyph, // the glyph from which to build the new character
                float xoffset, // horizontal offset of the new character within the rendered string
                lost::math::Vec2& pmin, // will be updated with new min values
                lost::math::Vec2& pmax); // will be updated with new max values

  uint32_t _size; // font size is not mutable after creation. Use the size() accessor to find out the size of the font.
};
}  
}

#endif
