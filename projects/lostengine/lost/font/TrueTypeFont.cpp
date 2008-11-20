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
    g->br = Vec2((g->rect.maxX()+1)/tw, g->rect.y/th);
    g->tl = Vec2(g->rect.x/tw, (g->rect.maxY()+1)/th);
    g->tr = Vec2((g->rect.maxX()+1)/tw, (g->rect.maxY()+1)/th);
  }
}

void TrueTypeFont::resetModel(boost::shared_ptr<Model> model, uint32_t numChars)
{
  uint32_t numVertsPerChar = 4;
  model->vertexCount = numChars*numVertsPerChar;
  uint32_t numCoordsPerVert = 2;
  model->vertices.reset(new float[model->vertexCount*numCoordsPerVert]); 
  model->texcoords.reset(new float[model->vertexCount*numCoordsPerVert]); 
  uint32_t trisPerChar = 2;
  uint32_t vertsPerTri = 3;
  model->indexCount = trisPerChar*vertsPerTri*numChars;
  model->indices.reset(new uint8_t[model->indexCount]);
  model->texture = atlas;
}

void TrueTypeFont::addGlyph(boost::shared_ptr<Model> model,
                            uint32_t index,
                            shared_ptr<Glyph> glyph,
                            float xoffset)
{
  Rect tr = glyph->rect; 
  tr.x = xoffset+glyph->xoffset;
  tr.y = glyph->yoffset;
  
  uint32_t indicesPerChar = 6; // 2 tris a 3 points
  uint32_t indexOffset = indicesPerChar*index;
  uint32_t vertsPerChar = 4;
  uint32_t vertsOffset = vertsPerChar*index;
  
  model->indices[indexOffset+0] = vertsOffset+0;
  model->indices[indexOffset+1] = vertsOffset+2;
  model->indices[indexOffset+2] = vertsOffset+3;
  model->indices[indexOffset+3] = vertsOffset+0;
  model->indices[indexOffset+4] = vertsOffset+1;
  model->indices[indexOffset+5] = vertsOffset+2;
  
  uint32_t coordsPerChar = 8; // 4 verts a 2 coords
  uint32_t coordOffset = coordsPerChar*index;
  
  model->vertices[coordOffset+0] = tr.x;
  model->vertices[coordOffset+1] = tr.y;
  model->vertices[coordOffset+2] = tr.maxX()+1;
  model->vertices[coordOffset+3] = tr.y;
  model->vertices[coordOffset+4] = tr.maxX()+1;
  model->vertices[coordOffset+5] = tr.maxY()+1;
  model->vertices[coordOffset+6] = tr.x;
  model->vertices[coordOffset+7] = tr.maxY()+1;
  
  model->texcoords[coordOffset+0] = glyph->bl.x;
  model->texcoords[coordOffset+1] = glyph->bl.y;
  model->texcoords[coordOffset+2] = glyph->br.x;
  model->texcoords[coordOffset+3] = glyph->br.y;
  model->texcoords[coordOffset+4] = glyph->tr.x;
  model->texcoords[coordOffset+5] = glyph->tr.y;
  model->texcoords[coordOffset+6] = glyph->tl.x;
  model->texcoords[coordOffset+7] = glyph->tl.y;  
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
  uint32_t numChars = inText.length();
  resetModel(result, numChars);
  float xoffset = 0;
  
  // kerning setup
  bool hasKerning = FT_HAS_KERNING(face->face());
  FT_UInt previousGlyphIndex, currentGlyphIndex;
  previousGlyphIndex = 0;
  FT_Vector kerningDelta;
  
  for(int i=0; i<numChars; ++i)
  {
    char c = inText[i];
    
    if(hasKerning)
    {
      currentGlyphIndex = FT_Get_Char_Index(face->face(), c);
      FT_Get_Kerning(face->face(), previousGlyphIndex, currentGlyphIndex, FT_KERNING_DEFAULT, &kerningDelta);
      signed long kerningoffset = kerningDelta.x>>6;
      xoffset+=kerningoffset;
      previousGlyphIndex = currentGlyphIndex;
    }
    
    shared_ptr<Glyph> glyph = char2size2glyph[c][inSizeInPoints];
    addGlyph(result, i, glyph, xoffset);
    xoffset+=glyph->advance;
  }
    
  return result;
}

}  
}
