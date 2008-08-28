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

        Face(FT_Face inFace);
        virtual ~Face();

        FT_Face face() { return mFace; }

        /** lets freetype render the specified glyph.
         * All memory management is performed by freetype. The result is placed in the
         * face->glyph->bitmap structure.
         */
        void renderGlyphToBitmap(unsigned long inSizeInPoints,
                                 char c);

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
                                  unsigned long& outTextureHeight);

        /** takes the current glyph of the face and converts it to a texture
         * with power-of-two measurements for backwwards compatibility.
         * A texture object must have been created and bound before calling this function.
         */
        void createGlyphPowerOfTwoTexture(boost::shared_ptr<lost::gl::Texture> tex, unsigned long& outTexWidth, unsigned long& outTexHeight);

        /** takes the current glyph of the face and converts it to a texture
         * with the exact same size as the bitmap. This only works on OpenGL >= 2.0 or if GL_ARB_texture_non_power_of_two
         * or similar was defined.
         * A texture object must have been created and bound before calling this function.
         */
        void createGlyphNonPowerOfTwoTexture(boost::shared_ptr<lost::gl::Texture> tex, unsigned long& outTexWidth, unsigned long& outTexHeight);

        /** always renders and creates a new glyph object.
         * if you want to us ethe cache, use getGlyph(...);
         *
         */
        boost::shared_ptr<lost::font::freetype::Glyph> renderGlyph(unsigned long inSizeInPoints, char inCharacter);

        /** performs a cache lookup for the requested glyph and returns the previously build variant or
         * renders a new one, caches and returns it.
         */
        boost::shared_ptr<lost::font::freetype::Glyph> getGlyph(unsigned long inSizeInPoints, char inCharacter);

        /** helper function for quick rendering of a string into a dispay list.
         * This is not really meant for anything else but short labels or one-liners.
         *
         * @param inSizeInPoints    size of text in points
         * @param inText            the text to render
         * @param outList           an existing list that's overwritten with the new content
         */
        void renderText(unsigned long inSizeInPoints,
                        const std::string& inText,
                        boost::shared_ptr<lost::gl::DisplayList>& outList);

        math::Rect rect() { return mRect; }

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
