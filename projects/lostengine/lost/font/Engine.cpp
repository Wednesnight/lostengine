#include "lost/font/Engine.h"
#include <boost/shared_ptr.hpp>
#include "lost/bitmap/Bitmap.h"

using namespace boost;
using namespace lost::bitmap;

namespace lost
{
namespace font
{
  
Engine::Engine()
{
}
  
Engine::~Engine()
{
}
 
  
boost::shared_ptr<bitmap::Bitmap>
Engine::renderGlyphToBitmap(boost::shared_ptr<freetype::Face> face,
                    uint32_t inSizeInPoints,
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
  
}
}
