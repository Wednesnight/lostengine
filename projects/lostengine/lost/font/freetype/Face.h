#ifndef LOST_FONT_FREETYPE_FACE_H
#define LOST_FONT_FREETYPE_FACE_H

#include "ft2build.h"
#include FT_FREETYPE_H
#include "lost/gl/gl.h"

#include "lost/common/Logger.h"
#include <boost/shared_ptr.hpp>
#include "lost/gl/Texture.h"
#include <stdexcept>
#include <string>
#include <boost/lexical_cast.hpp>
#include "lost/gl/Utils.h"
#include "lost/gl/PushAttrib.h"
#include "lost/gl/PushClientAttrib.h"
#include "lost/gl/DisplayList.h"
#include "lost/font/freetype/Glyph.h"
#include <map>
#include "lost/math/Rect.h"

namespace lost
{
  namespace font
  {
    namespace freetype
    {
      struct Face
      {
        typedef boost::shared_ptr<lost::font::freetype::Glyph> GlyphPtr;
        typedef std::map<char, GlyphPtr>                       CharToGlyphMap;
        typedef std::map<unsigned long, CharToGlyphMap >       GlyphCache;

        Face(FT_Face inFace)
        : mRect(0,0,0,0)
        {
          mFace = inFace;
          mPowerOfTwoTextures = true;
        }

        virtual ~Face()
        {
          FT_Error err = FT_Done_Face(mFace);
          if(err)
          {
            DOUT("FT_Done_Face error: "<<err);
          }
        }

        FT_Face face() { return mFace; }

        /** lets freetype render the specified glyph.
         * All memory management is performed by freetype. The result is placed in the
         * face->glyph->bitmap structure.
         */
        void renderGlyphToBitmap(unsigned long inSizeInPoints,
                                 char c)
        {
          FT_Error error = FT_Set_Char_Size(mFace, 0, inSizeInPoints*64, 72, 72);
          if(error) { throw std::runtime_error("FT_Set_Char_Size error: " + boost::lexical_cast<std::string>(error));}

          FT_UInt idx = FT_Get_Char_Index(mFace, c);
          FT_Int32 load_flags = 0;
          error = FT_Load_Glyph(mFace, idx, load_flags);
          if(error) {WOUT("FT_Load_Glyph error: " << error);}

          FT_Render_Mode render_mode = FT_RENDER_MODE_NORMAL;
          error = FT_Render_Glyph(mFace->glyph, render_mode);
          if(error) {WOUT("FT_Render_Glyph error: " << error);}
        }

        /** renders the provided character to a bitmap and converts this bitmap into a opengl texture.
         * Depending on the settings, the texture will be forced to a power-of-two texture and potentially use more
         * memory, but provide better backwards compatibility.
         *
         * @param inCharacter   the character to render
         * @param outTexture    receives a new texture object containing the converted bitmap data
         *
         * The following parameters are required for the calling code to correctly calculate the texture coordinates for
         * the target surface:
         *
         * @param outBitmapWidth    receives the original width of the freetype bitmap
         * @param outBitmapHeight   receives the original height of the freetype bitmap
         * @param outTextureWidth   receives the width of the resulting texture
         * @param outTextureHeight  receives the height of the resulting texture
         */
        // NOTE: set face size before rendering!
        void renderGlyphToTexture(unsigned long inSizeInPoints,
                                  char inCharacter,
                                  boost::shared_ptr<lost::gl::Texture>& outTexture,
                                  unsigned long& outBitmapWidth,
                                  unsigned long& outBitmapHeight,
                                  unsigned long& outTextureWidth,
                                  unsigned long& outTextureHeight)
        {
          // FIXME: I think renderGlyph should throw for undefined glyphs so we don't crash here
          renderGlyphToBitmap(inSizeInPoints, inCharacter); // let freetype render the glyph as 8 bit bitmap
                                  // we'll convert this bitmap to an opengl texture
          outTexture.reset(new lost::gl::Texture()); // also creates the opengl texture object
          outTexture->bind(); // all subsequent operations will occur on the now bound texture object

          // set up basic filter and wrap parameters
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_NEAREST to switch off filtering
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
          int clampparam = GL_CLAMP; // clamp the texture, don't repeat it if the coords are larger than 1s
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clampparam);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clampparam);

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
            createGlyphPowerOfTwoTexture(outTexture, outTextureWidth, outTextureHeight);
          }
          else
          {
            createGlyphNonPowerOfTwoTexture(outTexture, outTextureWidth, outTextureHeight);
          }

          outBitmapWidth = mFace->glyph->bitmap.width;
          outBitmapHeight = mFace->glyph->bitmap.rows;
        }



        /** takes the current glyph of the face and converts it to a texture
         * with power-of-two measurements for backwwards compatibility.
         * A texture object must have been created and bound before calling this function.
         */
        void createGlyphPowerOfTwoTexture(boost::shared_ptr<lost::gl::Texture> tex, unsigned long& outTexWidth, unsigned long& outTexHeight)
        {
          // find the power-of-two measurements for the current glyph
          // we need this to create power-of-two sized textures for opengl to be on the safe side and avoid side effects
          unsigned long potwidth = gl::utils::nextPowerOf2(mFace->glyph->bitmap.width);
          unsigned long potheight = gl::utils::nextPowerOf2(mFace->glyph->bitmap.rows);

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
                          mFace->glyph->bitmap.width,
                          mFace->glyph->bitmap.rows,
                          GL_ALPHA,
                          GL_UNSIGNED_BYTE,
                          mFace->glyph->bitmap.buffer);GLDEBUG_THROW;
          outTexWidth = potwidth;
          outTexHeight = potheight;
        }

        /** takes the current glyph of the face and converts it to a texture
         * with the exact same size as the bitmap. This only works on OpenGL >= 2.0 or if GL_ARB_texture_non_power_of_two
         * or similar was defined.
         * A texture object must have been created and bound before calling this function.
         */
        void createGlyphNonPowerOfTwoTexture(boost::shared_ptr<lost::gl::Texture> tex, unsigned long& outTexWidth, unsigned long& outTexHeight)
        {
          glTexImage2D(GL_TEXTURE_2D,
                       0,
                       GL_ALPHA,
                       mFace->glyph->bitmap.width,
                       mFace->glyph->bitmap.rows,
                       0,
                       GL_ALPHA,
                       GL_UNSIGNED_BYTE,
                       mFace->glyph->bitmap.buffer);GLDEBUG_THROW;
          outTexWidth = mFace->glyph->bitmap.width;
          outTexHeight = mFace->glyph->bitmap.rows;
        }

        /** always renders and creates a new glyph object.
         * if you want to us ethe cache, use getGlyph(...);
         *
         */
        boost::shared_ptr<lost::font::freetype::Glyph> renderGlyph(unsigned long inSizeInPoints, char inCharacter)
        {
          boost::shared_ptr<lost::gl::Texture> texture;
          unsigned long bitmapWidth;
          unsigned long bitmapHeight;
          unsigned long textureWidth;
          unsigned long textureHeight;

          renderGlyphToTexture(inSizeInPoints, inCharacter, texture, bitmapWidth, bitmapHeight, textureWidth, textureHeight);

          lost::math::Rect rect(0, 0, bitmapWidth, bitmapHeight);
          float maxU = static_cast<float>(bitmapWidth)/static_cast<float>(textureWidth);
          float maxV = static_cast<float>(bitmapHeight)/static_cast<float>(textureHeight);

          signed long xoffset = mFace->glyph->bitmap_left;
          signed long yoffset = mFace->glyph->bitmap_top - (mFace->glyph->metrics.height >> 6);
          signed long advance = (mFace->glyph->advance.x >> 6);
          boost::shared_ptr<lost::font::freetype::Glyph> result(new lost::font::freetype::Glyph(texture, rect, maxU, maxV, xoffset, yoffset, advance));
          return result;
        }

        /** performs a cache lookup for the requested glyph and returns the previously build variant or
         * renders a new one, caches and returns it.
         */
        boost::shared_ptr<lost::font::freetype::Glyph> getGlyph(unsigned long inSizeInPoints, char inCharacter)
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
            result = renderGlyph(inSizeInPoints, inCharacter);
            glyphCache[inSizeInPoints][inCharacter] = result;
          }

          return result;
        }

        /** helper function for quick rendering of a string into a dispay list.
         * This is not really meant for anything else but short labels or one-liners.
         *
         * @param inSizeInPoints    size of text in points
         * @param inText            the text to render
         * @param outList           an existing list that's overwritten with the new content
         */
        void renderText(unsigned long inSizeInPoints,
                        const std::string& inText,
                        boost::shared_ptr<lost::gl::DisplayList>& outList)
        {
          boost::shared_ptr<lost::font::freetype::Glyph> curglyph;
          unsigned long xoffset = 0;
          outList->begin();

          // kerning setup
          bool hasKerning = FT_HAS_KERNING(mFace);
          FT_UInt previousGlyphIndex, currentGlyphIndex;
          previousGlyphIndex = 0;
          FT_Vector kerningDelta;

          mRect.x      = 0;
          mRect.y      = 0;
          mRect.width  = 0;
          mRect.height = 0;

          // AAARGH! Note to self: don't access current glyph metrics from here when the glyph was actually fetched from cache and the "current" glyph is an old one.
          for(unsigned long i=0; i<inText.size(); ++i)
          {
            if(hasKerning)
            {
              currentGlyphIndex = FT_Get_Char_Index(mFace, inText[i]);
              FT_Get_Kerning(mFace, previousGlyphIndex, currentGlyphIndex, FT_KERNING_DEFAULT, &kerningDelta);
              signed long kerningoffset = kerningDelta.x>>6;
              xoffset+=kerningoffset;
              previousGlyphIndex = currentGlyphIndex;
            }

            curglyph = getGlyph(inSizeInPoints, inText[i]);
            curglyph->draw(xoffset+curglyph->xoffset(), curglyph->yoffset());
            xoffset     += curglyph->advance();
            mRect.height = std::max( mRect.height, curglyph->rect().height );
            mRect.y      = std::min( mRect.y, (float)curglyph->yoffset() );
          }
          mRect.width = xoffset;
          outList->end();
        }

        math::Rect rect()
        {
          return mRect;
        }

      private:
        FT_Face     mFace;
        bool        mPowerOfTwoTextures;
        GlyphCache  glyphCache;
        math::Rect  mRect;
      };
    }
  }
}

#endif
