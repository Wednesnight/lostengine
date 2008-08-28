#include "lost/font/freetype/Renderer.h"
#include "lost/gl/gl.h"
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include "lost/gl/Utils.h"
#include "lost/common/Logger.h"
#include "lost/gl/PushAttrib.h"
#include "lost/gl/PushClientAttrib.h"

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
  
  // set the appropriate pixel storage and transfer modes so the data is copied to the destination texture correctly
  // since freetype rendered to a 8bit buffer, we need some trickery to blow it up to the desired rgba texture
  // specifying GL_ALPHA means: the 8 bit source value is used as alpha, rgb components are set to zero.
  // this is NOT what we want! In order to draw correctly, the glyph should be white with the appropriate alpha channel
  // for anti-aliasing.
  // Therefore, we specify scale = 1 and bias = 1 for each color channel. Since all colors are initially zero, this results in
  // destcol = (srccol *scale)+bias = (0*1)+1 = 1
  // This means all pixels of the glyph end up being white, with only the alpha value controlling the actual drawing.
  // in order to prevent side effects, we need to push the attributes for this action
  lost::gl::PushAttrib attrib(GL_PIXEL_MODE_BIT);
  lost::gl::PushClientAttrib clientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
  glPixelStorei( GL_UNPACK_ROW_LENGTH, 0);GLDEBUG_THROW; // do not skip any additionaly components
  glPixelStorei( GL_UNPACK_ALIGNMENT, 1);GLDEBUG_THROW; // each value is a byte, hence the 1 byte alignment
  glPixelTransferf(GL_RED_SCALE, 1.0f);GLDEBUG_THROW; // component scale and bias as described above
  glPixelTransferf(GL_RED_BIAS, 1.0f);GLDEBUG_THROW;
  glPixelTransferf(GL_GREEN_SCALE, 1.0f);GLDEBUG_THROW;
  glPixelTransferf(GL_GREEN_BIAS, 1.0f);GLDEBUG_THROW;
  glPixelTransferf(GL_BLUE_SCALE, 1.0f);GLDEBUG_THROW;
  glPixelTransferf(GL_BLUE_BIAS, 1.0f);GLDEBUG_THROW;
  glPixelTransferf(GL_ALPHA_SCALE, 1.0f);GLDEBUG_THROW;
  glPixelTransferf(GL_ALPHA_BIAS, 0.0f);GLDEBUG_THROW;

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
  unsigned long potwidth = gl::utils::nextPowerOf2(face->face()->glyph->bitmap.width);
  unsigned long potheight = gl::utils::nextPowerOf2(face->face()->glyph->bitmap.rows);

  // call glTexImage2D once with pixels = 0 to allocate memory
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_ALPHA,
               potwidth,
               potheight,
               0,
               GL_ALPHA,
               GL_UNSIGNED_BYTE,
               0);GLDEBUG_THROW;

  // now we copy the actual data to the previsouly allocated texture
  glTexSubImage2D(GL_TEXTURE_2D,
                  0,
                  0,
                  0,
                  face->face()->glyph->bitmap.width,
                  face->face()->glyph->bitmap.rows,
                  GL_ALPHA,
                  GL_UNSIGNED_BYTE,
                  face->face()->glyph->bitmap.buffer);GLDEBUG_THROW;
  outTexWidth = potwidth;
  outTexHeight = potheight;
}

void Renderer::createGlyphNonPowerOfTwoTexture(boost::shared_ptr<Face> face,
                                               boost::shared_ptr<lost::gl::Texture> tex,
                                               unsigned long& outTexWidth,
                                               unsigned long& outTexHeight)
{
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_ALPHA,
               face->face()->glyph->bitmap.width,
               face->face()->glyph->bitmap.rows,
               0,
               GL_ALPHA,
               GL_UNSIGNED_BYTE,
               face->face()->glyph->bitmap.buffer);GLDEBUG_THROW;
  outTexWidth = face->face()->glyph->bitmap.width;
  outTexHeight = face->face()->glyph->bitmap.rows;
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