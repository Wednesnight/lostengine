#ifndef LOST_FONT_TRUETYPEFONT_H
#define LOST_FONT_TRUETYPEFONT_H

#include <boost/shared_ptr.hpp>
#include "lost/font/freetype/Library.h"
#include "lost/font/freetype/Face.h"
#include "lost/resource/File.h"
#include "lost/font/Model.h"
#include <string>
#include "lost/gl/Texture.h"
#include "lost/math/Vec2.h"
#include <list>

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
    char character; // the character code frmo which this glyph was generated
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
  
  boost::shared_ptr<Model> render(const std::string& inText);
  
  boost::shared_ptr<freetype::Face> face;
  boost::shared_ptr<freetype::Library> library;
  std::list<font::Model> models; // the font knows all models that have been rendered with it
};
}  
}

#endif