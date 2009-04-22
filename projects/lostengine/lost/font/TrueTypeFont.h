#ifndef LOST_FONT_TRUETYPEFONT_H
#define LOST_FONT_TRUETYPEFONT_H

#include <list>
#include <vector>
#include <string>
#include <map>
#include <fhtagn/text/transcoding.h>
#include <fhtagn/text/decoders.h>
#include "lost/font/freetype/Library.h"
#include "lost/font/freetype/Face.h"
#include "lost/resource/File.h"
#include "lost/font/Model.h"
#include "lost/gl/Texture.h"
#include "lost/math/Vec2.h"
#include "lost/bitmap/Bitmap.h"
#include "lost/gl/Texture.h"

namespace lost
{
namespace font
{
struct TrueTypeFont;
typedef lost::shared_ptr<TrueTypeFont> TrueTypeFontPtr;

struct TrueTypeFont
{
  struct Glyph
  {
    Glyph();
    ~Glyph();
    lost::math::Rect  rect; // the glyphs bitmaps rect inside the atlas texture
    // texture coordinates for drawing a quad
    lost::math::Vec2  bl;
    lost::math::Vec2  br;
    lost::math::Vec2  tl;
    lost::math::Vec2  tr;
    
    int32_t xoffset;
    int32_t yoffset;
    int32_t advance;
    
    bool drawable;
    
    lost::shared_ptr<bitmap::Bitmap> bitmap;
  };

  typedef lost::shared_ptr<Glyph> GlyphPtr;
  
  TrueTypeFont(freetype::LibraryPtr inLibrary,
               resource::FilePtr inFile);
  virtual ~TrueTypeFont();

  /** lets freetype render the specified glyph.
   * All memory management is performed by freetype. The result is placed in the
   * face->glyph->bitmap structure.
   */
  bitmap::BitmapPtr
  renderGlyphToBitmap(fhtagn::text::utf32_char_t c,
                      boost::uint32_t inSizeInPoints);
    
  /** renders the given string with this font and returns it as a textured mesh that
   *  has it's origin at 0,0.
   *
   *  @param inText         the text to render.
   *  @param inSizeInPoints pointsize for the rendered text.
   *
   */
  ModelPtr render(const fhtagn::text::utf32_string& inText,
                  boost::uint32_t inSizeInPoints);

  ModelPtr render(const std::string & inText,
                  boost::uint32_t inSizeInPoints);
  
  
  /** checks if the caches already contain the glyph for the given character 
   *  and updates it if they don't.
   *  @return true if the glyph was rendered, false if it was cached and didn't need to be rendered again.
   */
  bool renderGlyph(fhtagn::text::utf32_char_t c,
                   boost::uint32_t inSizeInPoints);
  
  /** builds a new texture atlas from the current glyphs in the cache.
   *  
   */
  void rebuildTextureAtlas();
  

  /** sets up internal model data structures so they can contain the given number of characters.
   */
  void resetModel(ModelPtr model, boost::uint32_t numChars);  

  void addGlyph(ModelPtr model,
                boost::uint32_t index,
                GlyphPtr glyph,
                float xoffset,
                lost::math::Vec2& pmin,
                lost::math::Vec2& pmax);

  /** returns the number of characters that are actually displayable 
   * and have a non-degenerate bitmap.
   * E.g. spaces will not be rendered as a mesh, but the following mesh segment
   * will simply be moved further.
   * Drawables will be flagged true, other will be flagged with drawable = false
   */
  boost::uint32_t countAndFlagDrawableChars(const fhtagn::text::utf32_string& inText,
                                            boost::uint32_t inSizeInPoints);
  
  freetype::FacePtr face;
  freetype::LibraryPtr library;

  lost::math::Vec2  atlasSize;
  
  std::map<fhtagn::text::utf32_char_t, std::map<boost::uint32_t, GlyphPtr> > char2size2glyph; 
  std::vector<GlyphPtr> glyphs; // this list of glyphs contains the glyphs in the order they were rendered
                                // this is important to preserve the ordering fo rth packing of the atlas

  gl::TexturePtr atlas;
};
}  
}

#endif
