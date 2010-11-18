#ifndef LOST_FONT_FREETYPE_FACE_H
#define LOST_FONT_FREETYPE_FACE_H

#include "lost/font/freetype/forward.h"
#include "lost/common/forward.h"
#include "lost/bitmap/forward.h"

namespace lost
{
  namespace font
  {
    namespace freetype
    {    
      struct Face
      {
        Face(LibraryPtr inLibrary, common::DataPtr inData);
        virtual ~Face();

        FT_Face face();
        bitmap::BitmapPtr renderGlyphToBitmap(uint32_t c, uint32_t inSizeInPoints);

        // some values derived from last rendered character
        // names don't necessarily match freetype terminology, but our own for mesh rendering
        // units in pixels
        int32_t xoffset();
        int32_t yoffset();
        int32_t advance();
        float ascender();
        float descender();
        float scale(); // this one not in pixels, used for asceder/Descender calculation
        
        bool hasKerning();
        // calculates the kerningoffset between previousGlyphIndex and currentGlyph in pixels
        // upon return previousGlyphIndex will contain the glyphindex of currentGlyph, so you can reuse it for the following call
        // initially, previousGlyphIndex should be set to 0 if you are starting a new string.
        int32_t kerningOffset(uint32_t& previousGlyphIndex, uint32_t currentGlyph);
      private:
        FT_Face     mFace;
        
        // font faces are only valid as long as the loaded data stays alive
        // so every font face object takes care of its own data
        common::DataPtr mFontData;
        
        // the freetype Library must ony be destroyed after all fonts were removed.
        // in order to avoid crashes and keep the fonts alive as long as possible,
        // we keep a reference to the library inside the font.
        LibraryPtr mLibrary;
      };
    }
  }
}

#endif
