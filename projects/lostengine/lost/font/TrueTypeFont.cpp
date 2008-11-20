#include "lost/font/TrueTypeFont.h"
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include "lost/common/Logger.h"
#include "lost/bitmap/Packer.h"
#include <stdexcept>

using namespace std;
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
  atlasSize.width = 256;
  atlasSize.height = 256;
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
      char2size2glyph[c][inSizeInPoints] = glyph;
      glyphs.push_back(glyph);
    }
    return result;
}

void TrueTypeFont::rebuildTextureAtlas()
{
  lost::bitmap::Packer::Result  packerResult;

  std::vector<boost::shared_ptr<lost::bitmap::Bitmap> > characterBitmaps;
  std::vector<boost::shared_ptr<Glyph> >::iterator i;
  for(i=glyphs.begin(); i!=glyphs.end(); ++i)
  {
    characterBitmaps.push_back((*i)->bitmap);
  }

  bitmap::Packer bitmapPacker;
  bitmapPacker.pack(packerResult,
                    atlasSize,
                    characterBitmaps,
                    Bitmap::COMPONENTS_RGBA, // bitmaps rendered byte the renderGlyphToBitmap method are always RGBA
                    false, // don't rotate rectbecause we don't support it yet
                    false); // don't sort because we don't want to alter position of previously rendered glyphs in texture
  
  // check if all bitmaps were packed, throw if not
  if(packerResult.rects.size() != characterBitmaps.size())
    throw runtime_error("couldn't pack all bitmaps in characterAtlas");
    
  // recreate atlas texture from current packed bitmap
  // glyphs that were rendered in a previous pass should end up in the same spot in the atlas since we forced the packer to omit sorting
  atlas.reset(new gl::Texture(packerResult.packedBitmap));
  
  // tell the glyphs where they are in the atlas texture in tex coords
  // the rects are reordered in the result, but the bitmapids are preserved in the vector of the same name
  for(uint32_t j=0; j<packerResult.rects.size(); ++j)
  {
    glyphs[packerResult.bitmapIds[j]]->rect = packerResult.rects[j];
  }
  
  // recalculate glyph texture coords
  // origin is bottom left corner, (0/0), for both bitmap and texture
  for(uint32_t k=0; k<glyphs.size(); ++k)
  {
    boost::shared_ptr<Glyph> g = glyphs[k];
    float tw = atlas->dataWidth;
    float th = atlas->dataHeight;
    g->bl = Vec2(g->rect.x/tw, g->rect.y/th);
    g->br = Vec2(g->rect.maxX()/tw, g->rect.y/th);
    g->tl = Vec2(g->rect.x/tw, g->rect.maxY()/th);
    g->tr = Vec2(g->rect.maxX()/tw, g->rect.maxY()/th);
  }
}

shared_ptr<Model> TrueTypeFont::render(const std::string& inText,
                                       uint32_t inSizeInPoints)
{
  shared_ptr<Model>  result(new Model);
  
  // render glyphs if required
  uint32_t renderedGlyphs = 0;
  for(int i=0; i<inText.length(); ++i)
  {
    if(renderGlyph(inText[i], inSizeInPoints))
      ++renderedGlyphs;
  }
  
  // rebuild atlas if any new glyphs were rendered
  if(renderedGlyphs > 0)
    rebuildTextureAtlas();
  
  // build model from scratch with the infos gathered 
  
  boost::shared_ptr<Glyph> g = char2size2glyph[inText[0]][inSizeInPoints];
  Rect tr = g->rect; 
  tr.x = 0;
  tr.y = 0;
  
  result->vertexCount = 4;
  result->vertices.reset(new float[result->vertexCount*2]); // x/y coord per vertex
  result->texcoords.reset(new float[result->vertexCount*2]); // u/v coord per vertex
  result->indexCount = 6; // 2 tris for a start
  result->indices.reset(new uint8_t[result->indexCount]);
  result->indices[0] = 0;
  result->indices[1] = 2;
  result->indices[2] = 3;
  result->indices[3] = 0;
  result->indices[4] = 1;
  result->indices[5] = 2;

  result->vertices[0] = tr.x;
  result->vertices[1] = tr.y;
  result->vertices[2] = tr.maxX();
  result->vertices[3] = tr.y;
  result->vertices[4] = tr.maxX();
  result->vertices[5] = tr.maxY();
  result->vertices[6] = tr.x;
  result->vertices[7] = tr.maxY();

  result->texcoords[0] = g->bl.x;
  result->texcoords[1] = g->bl.y;
  result->texcoords[2] = g->br.x;
  result->texcoords[3] = g->br.y;
  result->texcoords[4] = g->tr.x;
  result->texcoords[5] = g->tr.y;
  result->texcoords[6] = g->tl.x;
  result->texcoords[7] = g->tl.y;
    
  return result;
}

}  
}
