#include "lost/font/TrueTypeFont.h"
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include "lost/common/Logger.h"

using namespace boost;
using namespace lost::bitmap;
using namespace lost::font::freetype;
using namespace lost::math;

namespace lost
{
namespace font
{
    
TrueTypeFont::TrueTypeFont(boost::shared_ptr<freetype::Library> inLibrary,
             boost::shared_ptr<resource::File> inFile)
{
  face.reset(new Face(inLibrary, inFile));
}

TrueTypeFont::~TrueTypeFont()
{
}

boost::shared_ptr<bitmap::Bitmap>
TrueTypeFont::renderGlyphToBitmap(uint32_t inSizeInPoints,
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
  
  shared_ptr<Bitmap> result(new Bitmap(face->face()->glyph->bitmap.width,
                                       face->face()->glyph->bitmap.rows,
                                       Bitmap::COMPONENTS_RGBA,
                                       Bitmap::COMPONENTS_ALPHA,
                                       face->face()->glyph->bitmap.buffer));
  
  
  return result;
}
  
  
shared_ptr<Model> TrueTypeFont::render(const std::string& inText)
{
  shared_ptr<Model>  result;
  
  // --- add characters of incoming string to current character buffer
  // --- create bitmaps for characters that don't have one yet
  // --- stuff new bitmaps into atlas, reshuffle atlas if necessary
  // --- 
  
  
  return result;
}

}  
}
