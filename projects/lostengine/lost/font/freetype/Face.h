/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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
        Face(LibraryPtr inLibrary, common::DataPtr inData, uint32_t inSizeInPoints);
        virtual ~Face();

        FT_Face face();
        bitmap::BitmapPtr renderGlyphToBitmap(uint32_t c, uint32_t inSizeInPoints);

        uint32_t sizeInPoints;
        
        
        // some values derived from last rendered character
        // names don't necessarily match freetype terminology, but our own for mesh rendering
        // units in pixels
        int32_t xoffset();
        int32_t yoffset();
        int32_t advance();
        float ascender();
        float descender();
        float scale(); // this one not in pixels, used for asceder/Descender calculation

        float height(); // height of one line or baseline distance
        float linegap(); // distance between lines
        
        bool hasKerning();
        // calculates the kerningoffset between previousGlyphIndex and currentGlyph in pixels
        // upon return previousGlyphIndex will contain the glyphindex of currentGlyph, so you can reuse it for the following call
        // initially, previousGlyphIndex should be set to 0 if you are starting a new string.
        int32_t kerningOffset(uint32_t& previousGlyphIndex, uint32_t currentGlyph);
      private:
        bool        _hasKerning;
        float       _linegap;
        float       _height;
        float       _baselineDistance;
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
