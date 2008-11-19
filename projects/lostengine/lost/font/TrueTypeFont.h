#ifndef LOST_FONT_TRUETYPEFONT_H
#define LOST_FONT_TRUETYPEFONT_H

#include <boost/shared_ptr.hpp>
#include <list>
#include <string>
#include <map>
#include "lost/font/freetype/Library.h"
#include "lost/font/freetype/Face.h"
#include "lost/resource/File.h"
#include "lost/font/Model.h"
#include "lost/gl/Texture.h"
#include "lost/math/Vec2.h"
#include "lost/bitmap/Bitmap.h"

namespace lost
{
namespace font
{
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
    
    boost::shared_ptr<bitmap::Bitmap> bitmap;
    boost::shared_ptr<gl::Texture> texture; // the texture that holds the rendered glyph    
  };
  
  TrueTypeFont(boost::shared_ptr<freetype::Library> inLibrary,
               boost::shared_ptr<resource::File> inFile);
  virtual ~TrueTypeFont();

  /** lets freetype render the specified glyph.
   * All memory management is performed by freetype. The result is placed in the
   * face->glyph->bitmap structure.
   */
  boost::shared_ptr<bitmap::Bitmap>
  renderGlyphToBitmap(char c,
                      uint32_t inSizeInPoints);
    
  /** renders the given string with this font and returns it as a textured mesh that
   *  has it's origin at 0,0.
   *
   *  @param inText         the text to render.
   *  @param inSizeInPoints pointsize for the rendered text.
   *
   */
  boost::shared_ptr<Model> render(const std::string& inText,
                                  uint32_t inSizeInPoints);
  
  
  /** checks if the caches already contain the glyph for the given character 
   *  and updates it if they don't.
   *  @return true if the glyph was rendered, false if it was cached and didn't need to be rendered again.
   */
  bool renderGlyph(char c,
                   uint32_t inSizeInPoints);
  
  /** builds a new texture atlas from the current glyphs in the cache.
   *  
   */
  void rebuildTextureAtlas();
  
  
  boost::shared_ptr<freetype::Face> face;
  boost::shared_ptr<freetype::Library> library;
  std::list<font::Model> models; // the font knows all models that have been rendered with it

  lost::math::Vec2  atlasSize;
  
  std::map<char, std::map<uint32_t, boost::shared_ptr<Glyph> > > char2size2glyph; 
};
}  
}

#endif