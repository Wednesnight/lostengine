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

TrueTypeFont::Glyph::Glyph()
{
}

TrueTypeFont::Glyph::~Glyph()
{
}

TrueTypeFont::TrueTypeFont(boost::shared_ptr<freetype::Library> inLibrary,
             boost::shared_ptr<resource::File> inFile)
{
  face.reset(new Face(inLibrary, inFile));
}

TrueTypeFont::~TrueTypeFont()
{
}

boost::shared_ptr<bitmap::Bitmap>
TrueTypeFont::renderGlyphToBitmap(char c,
                                  uint32_t inSizeInPoints)
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
  result->flip();  
  return result;
}
  
bool TrueTypeFont::renderGlyph(char c,
                               uint32_t inSizeInPoints)
{
    bool result = false;
    boost::shared_ptr<Glyph> glyph = char2size2glyph[c][inSizeInPoints];
    if(!glyph) 
    {
      result = true;
      glyph.reset(new Glyph);
      glyph->bitmap = renderGlyphToBitmap(c, inSizeInPoints);
      
      glyph->xoffset = face->face()->glyph->bitmap_left;
      glyph->yoffset = face->face()->glyph->bitmap_top - (face->face()->glyph->metrics.height >> 6);
      glyph->advance = (face->face()->glyph->advance.x >> 6);
      
    }
    return result;
}

void TrueTypeFont::rebuildTextureAtlas()
{
}

shared_ptr<Model> TrueTypeFont::render(const std::string& inText,
                                       uint32_t inSizeInPoints)
{
  shared_ptr<Model>  result(new Model);
  
  // two passes: 1.) render glyphs 2.) create meshes with proper geometry and kerning
  uint32_t renderedGlyphs = 0;
  for(int i=0; i<inText.length(); ++i)
  {
    if(renderGlyph(inText[i], inSizeInPoints))
      ++renderedGlyphs;
  }
  
  if(renderedGlyphs > 0)
    rebuildTextureAtlas();
  
  return result;
}

}  
}
