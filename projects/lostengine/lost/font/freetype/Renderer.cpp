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

namespace lost
{
namespace font
{
namespace freetype
{

Renderer::Renderer()
{
  mPowerOfTwoTextures = true;
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

void Renderer::renderGlyphToTexture(boost::shared_ptr<Face> face,
                                    unsigned long inSizeInPoints,
                                    char inCharacter,
                                    boost::shared_ptr<lost::gl::Texture>& outTexture,
                                    unsigned long& outBitmapWidth,
                                    unsigned long& outBitmapHeight,
                                    unsigned long& outTextureWidth,
                                    unsigned long& outTextureHeight)
{
  // FIXME: I think renderGlyph should throw for undefined glyphs so we don't crash here
  renderGlyphToBitmap(face, inSizeInPoints, inCharacter); // let freetype render the glyph as 8 bit bitmap
                          // we'll convert this bitmap to an opengl texture
  outTexture.reset(new lost::gl::Texture()); // also creates the opengl texture object
  outTexture->bind(); // all subsequent operations will occur on the now bound texture object
  outTexture->filter(GL_NEAREST);
  outTexture->wrap(GL_CLAMP_TO_EDGE);
  
  if(mPowerOfTwoTextures)
  {
    createGlyphPowerOfTwoTexture(face, outTexture, outTextureWidth, outTextureHeight);
  }
  else
  {
    createGlyphNonPowerOfTwoTexture(face, outTexture, outTextureWidth, outTextureHeight);
  }

  outBitmapWidth = face->face()->glyph->bitmap.width;
  outBitmapHeight = face->face()->glyph->bitmap.rows;
}

void Renderer::createGlyphPowerOfTwoTexture(boost::shared_ptr<Face> face,
                                            boost::shared_ptr<lost::gl::Texture> tex,
                                            unsigned long& outTexWidth,
                                            unsigned long& outTexHeight)
{
  // find the power-of-two measurements for the current glyph
  // we need this to create power-of-two sized textures for opengl to be on the safe side and avoid side effects
  unsigned long potwidth = nextPowerOf2(face->face()->glyph->bitmap.width);
  unsigned long potheight = nextPowerOf2(face->face()->glyph->bitmap.rows);

  shared_ptr<Bitmap> bmp(new Bitmap(face->face()->glyph->bitmap.width,
                                      face->face()->glyph->bitmap.rows,
                                      Bitmap::COMPONENTS_RGBA,
                                      Bitmap::COMPONENTS_ALPHA,
                                      face->face()->glyph->bitmap.buffer));
  
  // call glTexImage2D once with pixels = 0 to allocate memory
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA,
               potwidth,
               potheight,
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               0);GLDEBUG_THROW;

  // now we copy the actual data to the previsouly allocated texture
  glTexSubImage2D(GL_TEXTURE_2D,
                  0,
                  0,
                  0,
                  bmp->width,
                  bmp->height,
                  GL_RGBA,
                  GL_UNSIGNED_BYTE,
                  bmp->data);GLDEBUG_THROW;
  outTexWidth = potwidth;
  outTexHeight = potheight;
}

void Renderer::createGlyphNonPowerOfTwoTexture(boost::shared_ptr<Face> face,
                                               boost::shared_ptr<lost::gl::Texture> tex,
                                               unsigned long& outTexWidth,
                                               unsigned long& outTexHeight)
{
  shared_ptr<Bitmap> bmp(new Bitmap(face->face()->glyph->bitmap.width,
                                    face->face()->glyph->bitmap.rows,
                                    Bitmap::COMPONENTS_RGBA,
                                    Bitmap::COMPONENTS_ALPHA,
                                    face->face()->glyph->bitmap.buffer));  
  
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA,
               bmp->width,
               bmp->height,
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               bmp->data);GLDEBUG_THROW;
  outTexWidth = bmp->width;
  outTexHeight = bmp->height;
}

boost::shared_ptr<lost::font::freetype::Glyph> Renderer::renderGlyph(boost::shared_ptr<Face> face,
                                                                     unsigned long inSizeInPoints,
                                                                     char inCharacter)
{
  boost::shared_ptr<lost::gl::Texture> texture;
  unsigned long bitmapWidth;
  unsigned long bitmapHeight;
  unsigned long textureWidth;
  unsigned long textureHeight;

  renderGlyphToTexture(face, inSizeInPoints, inCharacter, texture, bitmapWidth, bitmapHeight, textureWidth, textureHeight);

  lost::math::Rect rect(0, 0, bitmapWidth, bitmapHeight);
  float maxU = static_cast<float>(bitmapWidth)/static_cast<float>(textureWidth);
  float maxV = static_cast<float>(bitmapHeight)/static_cast<float>(textureHeight);

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