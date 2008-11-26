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
  drawable = true;
  xoffset = 0;
  yoffset = 0;
  advance = 0;
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
  std::vector<uint32_t> glyphIndex; // because we might not use all bitmaps, need to preserve the original indices of the glyphs
  std::vector<boost::shared_ptr<Glyph> >::iterator i;
  uint32_t counter = 0;
  for(i=glyphs.begin(); i!=glyphs.end(); ++i)
  {
    // only use bitmaps that have width and height != 0
    // spaces might be rendered as 0/0 bitmaps, but have a valid advance value
    // that we need to render the string correctly.
    shared_ptr<Bitmap> bmp = (*i)->bitmap;
    if((bmp->width != 0) && (bmp->height != 0))
    {
      characterBitmaps.push_back(bmp);
      glyphIndex.push_back(counter);
    }
    counter++;
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
    glyphs[glyphIndex[packerResult.bitmapIds[j]]]->rect = packerResult.rects[j];
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
                            float xoffset,
                            lost::math::Vec2& pmin,
                            lost::math::Vec2& pmax)
{
  Rect tr = glyph->rect; 
  tr.x = xoffset+glyph->xoffset;
  tr.y = glyph->yoffset;
  
  pmin.x = min(pmin.x, tr.x);
  pmin.y = min(pmin.y, tr.y);

  pmax.x = max(pmax.x, tr.maxX());
  pmax.y = max(pmax.y, tr.maxY());
  
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

uint32_t TrueTypeFont::countAndFlagDrawableChars(const std::string& inText,
                                            uint32_t inSizeInPoints)
{
  uint32_t result = 0;
  
  for(uint32_t i=0; i<inText.size(); ++i)
  {
    shared_ptr<Glyph> glyph = char2size2glyph[inText[i]][inSizeInPoints];
    if(glyph && glyph->bitmap && (glyph->bitmap->width > 0) && (glyph->bitmap->height > 0))
    {
      result++;
      glyph->drawable = true;
    }
    else
    {
      if(glyph)
        glyph->drawable = false;
    }
  }
  
  return result;
}

  
shared_ptr<Model> TrueTypeFont::render(const std::string& inText,
                                       uint32_t inSizeInPoints)
{
  DOUT("rendering "<<inText<<" with size "<<inSizeInPoints<<" atlas size: "<<atlasSize);
  shared_ptr<Model>  result(new Model);
  
  // render glyphs if required
  uint32_t renderedGlyphs = 0;
  for(int i=0; i<inText.length(); ++i)
  {
    if(renderGlyph(inText[i], inSizeInPoints))
      ++renderedGlyphs;
  }
  DOUT("rendered "<<renderedGlyphs<<" new glyphs");
  // rebuild atlas if any new glyphs were rendered
  if(renderedGlyphs > 0)
    rebuildTextureAtlas();
  
  // build model from scratch with the infos gathered 
  uint32_t drawableChars = countAndFlagDrawableChars(inText, inSizeInPoints);
  resetModel(result, drawableChars);
  float xoffset = 0;
  
  // kerning setup
  bool hasKerning = FT_HAS_KERNING(face->face());
  FT_UInt previousGlyphIndex, currentGlyphIndex;
  previousGlyphIndex = 0;
  FT_Vector kerningDelta;
  
  // size calculation 
  Vec2 pmin, pmax; 
  uint32_t numChars = inText.size();
  uint32_t addIndex=0; // we iterate over all chracters, but not all of them might be drawable
                       // so we need a separate index for the actual insertion of a character into the mesh
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
    if(glyph && glyph->drawable)
    {
      addGlyph(result, addIndex, glyph, xoffset, pmin, pmax);
      addIndex++;
    }
    xoffset+=glyph->advance;
  }
  
  result->size.width = (pmax.x-pmin.x)+1;  
  result->size.height = (pmax.y-pmin.y)+1;  
  return result;
}

}  
}
