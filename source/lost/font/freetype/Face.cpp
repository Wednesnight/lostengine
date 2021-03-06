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

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_TRUETYPE_TABLES_H
#include <stdexcept>
#include <sstream>
#include "lost/font/freetype/Face.h"
#include "lost/common/Logger.h"
#include "lost/common/Data.h"
#include "lost/font/freetype/Library.h"
#include "lost/bitmap/Bitmap.h"

namespace lost
{
namespace font
{
namespace freetype
{

using namespace lost::bitmap;

Face::Face(LibraryPtr inLibrary,
           common::DataPtr inData,
           uint32_t inSizeInPoints)
{
  FT_Error error  = FT_New_Memory_Face(inLibrary->library,
                                       reinterpret_cast<FT_Byte*>(inData->bytes.get()),
                                       inData->size,
                                       0,
                                       &mFace);
  if(error)
  {
    std::ostringstream os;
    os << "FT_New_Face error: " << error;
    throw std::runtime_error(os.str());
  }

  mFontData = inData;
  mLibrary = inLibrary;

  error = FT_Set_Char_Size(mFace, 0, inSizeInPoints*64, 72, 72);
  if(error)
  {
    std::ostringstream os;
    os << error;
    throw std::runtime_error("FT_Set_Char_Size error: " + os.str());
  }
  sizeInPoints = inSizeInPoints;
  _hasKerning = FT_HAS_KERNING(mFace) != 0;
}

Face::~Face()
{
  FT_Error err = FT_Done_Face(mFace);
  if(err)
  {
    DOUT("FT_Done_Face error: "<<err);
  }
}

FT_Face Face::face()
{
  return mFace;
}

bitmap::BitmapPtr
Face::renderGlyphToBitmap(uint32_t c,
                          uint32_t inSizeInPoints)
{  
  FT_UInt idx = FT_Get_Char_Index(mFace, c);
  FT_Int32 load_flags = 0;
  FT_Error error = FT_Load_Glyph(mFace, idx, load_flags);
  if(error) {WOUT("FT_Load_Glyph error: " << error);}
  
  FT_Render_Mode render_mode = FT_RENDER_MODE_NORMAL;
  error = FT_Render_Glyph(mFace->glyph, render_mode);
  if(error) {WOUT("FT_Render_Glyph error: " << error);}
  
  BitmapPtr result(new Bitmap(mFace->glyph->bitmap.width,
                              mFace->glyph->bitmap.rows,
                              bitmap::COMPONENTS_RGBA,
                              bitmap::COMPONENTS_ALPHA,
                              mFace->glyph->bitmap.buffer));  
  result->flip();  
  return result;
}

int32_t Face::xoffset() { return mFace->glyph->bitmap_left; }
int32_t Face::yoffset() { return mFace->glyph->bitmap_top - (mFace->glyph->metrics.height >> 6); }
int32_t Face::advance() { return mFace->glyph->advance.x >> 6; }
float Face::scale() { return ((float)mFace->size->metrics.y_scale)/65536.0f; }
float Face::ascender() { return (((float)mFace->ascender)/64.0f)*scale(); }
float Face::descender() { return (((float)mFace->descender)/64.0f)*scale(); }
float Face::height() { return (((float)mFace->height)/64.0f)*scale(); }
float Face::linegap() { return height() - (ascender() - descender()); }

bool Face::hasKerning()
{
  return _hasKerning;
}

int32_t Face::kerningOffset(uint32_t& previousGlyphIndex, uint32_t currentGlyph)
{
  FT_Vector kerningDelta;
  FT_UInt currentGlyphIndex = FT_Get_Char_Index(mFace, currentGlyph);
  FT_Get_Kerning(mFace, previousGlyphIndex, currentGlyphIndex, FT_KERNING_DEFAULT, &kerningDelta);
  previousGlyphIndex = currentGlyphIndex;
  return kerningDelta.x>>6;
}

}
}
}
