#ifndef LOST_FONT_TRUETYPEFONT_H
#define LOST_FONT_TRUETYPEFONT_H

#include <boost/shared_ptr.hpp>
#include <list>
#include <vector>
#include <string>
#include <map>
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
    
    boost::shared_ptr<bitmap::Bitmap> bitmap;
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
                      boost::uint32_t inSizeInPoints);
    
  /** renders the given string with this font and returns it as a textured mesh that
   *  has it's origin at 0,0.
   *
   *  @param inText         the text to render.
   *  @param inSizeInPoints pointsize for the rendered text.
   *
   */
  boost::shared_ptr<Model> render(const std::string& inText,
                                  boost::uint32_t inSizeInPoints);
  
  
  /** checks if the caches already contain the glyph for the given character 
   *  and updates it if they don't.
   *  @return true if the glyph was rendered, false if it was cached and didn't need to be rendered again.
   */
  bool renderGlyph(char c,
                   boost::uint32_t inSizeInPoints);
  
  /** builds a new texture atlas from the current glyphs in the cache.
   *  
   */
  void rebuildTextureAtlas();
  

    /** sets up internal model data structures so they can contain the given number of characters.
     */
    void resetModel(boost::shared_ptr<Model> model, boost::uint32_t numChars);  

  void addGlyph(boost::shared_ptr<Model> model,
                boost::uint32_t index,
                boost::shared_ptr<Glyph> glyph,
                float xoffset,
                lost::math::Vec2& pmin,
                lost::math::Vec2& pmax);
  /** returns the number of characters that are actually displayable 
   * and have a non-degenerate bitmap.
   * E.g. spaces will not be rendered as a mesh, but the following mesh segment
   * will simply be moved further.
   * Drawables will be flagged true, other will be flagged with drawable = false
   */
  boost::uint32_t countAndFlagDrawableChars(const std::string& inText, boost::uint32_t inSizeInPoints);
  
  boost::shared_ptr<freetype::Face> face;
  boost::shared_ptr<freetype::Library> library;

  lost::math::Vec2  atlasSize;
  
  std::map<char, std::map<boost::uint32_t, boost::shared_ptr<Glyph> > > char2size2glyph; 
  std::vector<boost::shared_ptr<Glyph> > glyphs; // this list of glyphs contains the glyphs in the order they were rendered
                                                 // this is important to preserve the ordering fo rth packing of the atlas

  boost::shared_ptr<gl::Texture> atlas;
};
}  
}

#endif
