#ifndef LOST_FONT_TRUETYPEFONT_H
#define LOST_FONT_TRUETYPEFONT_H

#include <boost/shared_ptr.hpp>
#include <list>
#include <string>
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
    // texture coordinates for drawing a quad
    lost::math::Vec2  bl;
    lost::math::Vec2  br;
    lost::math::Vec2  tl;
    lost::math::Vec2  tr;
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
  renderGlyphToBitmap(uint32_t inSizeInPoints,
                      char c);
    
    
  boost::shared_ptr<Model> render(const std::string& inText);
  
  boost::shared_ptr<freetype::Face> face;
  boost::shared_ptr<freetype::Library> library;
  std::list<font::Model> models; // the font knows all models that have been rendered with it

  lost::math::Vec2  atlasMinSize;
  lost::math::Vec2  atlasMaxSize;
  
  
//  std::map<char, std::map<uint32_t, Glyph> > char2size2glyph; 
};
}  
}

#endif