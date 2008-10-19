#include "lost/font/freetype/Renderer.h"
#include "lost/gl/gl.h"
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include "lost/gl/Utils.h"
#include "lost/common/Logger.h"
#include "lost/gl/PushAttrib.h"
#include "lost/gl/PushClientAttrib.h"
#include "lost/bitmap/Bitmap.h"

using namespace lost::math;
using namespace boost;
using namespace lost::bitmap;
using namespace lost::gl;

namespace lost
{
namespace font
{
namespace freetype
{

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::renderGlyphToBitmap(boost::shared_ptr<Face> face,
                                   unsigned long inSizeInPoints,
                                   char c)
{
  FT_Error error = FT_Set_Char_Size(face->face(), 0, inSizeInPoints*64, 72, 72);
  if(error) { throw std::runtime_error("FT_Set_Char_Size error: " + boost::lexical_cast<std::string>(error));}

  FT_UInt idx = FT_Get_Char_Index(face->face(), c);
  FT_Int32 load_flags = 0;
  error = FT_Load_Glyph(face->face(), idx, load_flags);
  if(error) {WOUT("FT_Load_Glyph error: " << error);}

  FT_Render_Mode render_mode = FT_RENDER_MODE_NORMAL;
  error = FT_Render_Glyph(face->face()->glyph, render_mode);
  if(error) {WOUT("FT_Render_Glyph error: " << error);}
}

boost::shared_ptr<lost::gl::Texture> Renderer::renderGlyphToTexture(boost::shared_ptr<Face> face,
                                    unsigned long inSizeInPoints,
                                    char inCharacter)
{
  // FIXME: I think renderGlyph should throw for undefined glyphs so we don't crash here
  renderGlyphToBitmap(face, inSizeInPoints, inCharacter); // let freetype render the glyph as 8 bit bitmap

  shared_ptr<Bitmap> bmp(new Bitmap(face->face()->glyph->bitmap.width,
                                    face->face()->glyph->bitmap.rows,
                                    Bitmap::COMPONENTS_RGBA,
                                    Bitmap::COMPONENTS_ALPHA,
                                    face->face()->glyph->bitmap.buffer));

  boost::shared_ptr<lost::gl::Texture> result(new lost::gl::Texture(bmp));
  return result;
}

boost::shared_ptr<lost::font::freetype::Glyph> Renderer::renderGlyph(boost::shared_ptr<Face> face,
                                                                     unsigned long inSizeInPoints,
                                                                     char inCharacter)
{
  boost::shared_ptr<lost::gl::Texture> texture = renderGlyphToTexture(face, inSizeInPoints, inCharacter);

  lost::math::Rect rect(0, 0, texture->dataWidth, texture->dataHeight);
  float maxU = static_cast<float>(texture->dataWidth)/static_cast<float>(texture->width);
  float maxV = static_cast<float>(texture->dataHeight)/static_cast<float>(texture->height);

  signed long xoffset = face->face()->glyph->bitmap_left;
  signed long yoffset = face->face()->glyph->bitmap_top - (face->face()->glyph->metrics.height >> 6);
  signed long advance = (face->face()->glyph->advance.x >> 6);
  boost::shared_ptr<lost::font::freetype::Glyph> result(new lost::font::freetype::Glyph(texture, rect, maxU, maxV, xoffset, yoffset, advance));
  return result;
}

boost::shared_ptr<lost::font::freetype::Glyph> Renderer::getGlyph(boost::shared_ptr<Face> face,
                                                                  unsigned long inSizeInPoints,
                                                                  char inCharacter)
{
  boost::shared_ptr<lost::font::freetype::Glyph> result;

  GlyphCache::iterator gcpos;
  CharToGlyphMap::iterator ctgpos;
  gcpos = glyphCache.find(inSizeInPoints);
  if(
      (gcpos != glyphCache.end()) &&                                      // means we have a cache for the requested size
      ((ctgpos = gcpos->second.find(inCharacter)) != gcpos->second.end()) // means theres a glyph for the character in the requested size
    )
  {
    result = ctgpos->second;
  }
  else
  {
    result = renderGlyph(face, inSizeInPoints, inCharacter);
    glyphCache[inSizeInPoints][inCharacter] = result;
  }

  return result;
}

void Renderer::renderText(boost::shared_ptr<Face> face,
                          unsigned long inSizeInPoints,
                          const std::string& inText,
                          boost::shared_ptr<lost::gl::DisplayList>& outList)
{
  boost::shared_ptr<lost::font::freetype::Glyph> curglyph;
  unsigned long xoffset = 0;
  outList->begin();

  // kerning setup
  bool hasKerning = FT_HAS_KERNING(face->face());
  FT_UInt previousGlyphIndex, currentGlyphIndex;
  previousGlyphIndex = 0;
  FT_Vector kerningDelta;

  // AAARGH! Note to self: don't access current glyph metrics from here when the glyph was actually 
  // fetched from cache and the "current" glyph is an old one.
  for(unsigned long i=0; i<inText.size(); ++i)
  {
    if(hasKerning)
    {
      currentGlyphIndex = FT_Get_Char_Index(face->face(), inText[i]);
      FT_Get_Kerning(face->face(), previousGlyphIndex, currentGlyphIndex, FT_KERNING_DEFAULT, &kerningDelta);
      signed long kerningoffset = kerningDelta.x>>6;
      xoffset+=kerningoffset;
      previousGlyphIndex = currentGlyphIndex;
    }

    curglyph = getGlyph(face, inSizeInPoints, inText[i]);
    curglyph->draw(xoffset+curglyph->xoffset(), curglyph->yoffset());
    xoffset     += curglyph->advance();
  }
  outList->end();
}

}
}
}