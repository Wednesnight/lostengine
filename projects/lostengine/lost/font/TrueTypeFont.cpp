#include "ft2build.h"
#include FT_FREETYPE_H
#include "lost/font/TrueTypeFont.h"
#include "lost/common/Logger.h"
#include "lost/bitmap/Packer.h"
#include "lost/gl/Texture.h"
#include <stdexcept>
#include <fhtagn/text/transcoding.h>
#include <fhtagn/text/decoders.h>
#include "lost/font/freetype/Library.h"
#include "lost/font/freetype/Face.h"
#include "lost/font/RenderedText.h"
#include "lost/bitmap/Bitmap.h"


using namespace std;
using namespace lost::bitmap;
using namespace lost::font::freetype;
using namespace lost::math;

namespace ftxt = fhtagn::text;

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

TrueTypeFont::TrueTypeFont(freetype::LibraryPtr inLibrary,
                           common::DataPtr inData)
{
  face.reset(new Face(inLibrary, inData));
  atlasSize.width = 256;
  atlasSize.height = 256;
}

TrueTypeFont::~TrueTypeFont()
{
}

bitmap::BitmapPtr
TrueTypeFont::renderGlyphToBitmap(ftxt::utf32_char_t c,
                                  uint32_t inSizeInPoints)
{
  FT_Error error = FT_Set_Char_Size(face->face(), 0, inSizeInPoints*64, 72, 72);
  if(error)
  {
    ostringstream os;
    os << error;
    throw std::runtime_error("FT_Set_Char_Size error: " + os.str());
  }
  
  FT_UInt idx = FT_Get_Char_Index(face->face(), c);
  FT_Int32 load_flags = 0;
  error = FT_Load_Glyph(face->face(), idx, load_flags);
  if(error) {WOUT("FT_Load_Glyph error: " << error);}
  
  FT_Render_Mode render_mode = FT_RENDER_MODE_NORMAL;
  error = FT_Render_Glyph(face->face()->glyph, render_mode);
  if(error) {WOUT("FT_Render_Glyph error: " << error);}
  
  BitmapPtr result(new Bitmap(face->face()->glyph->bitmap.width,
                               face->face()->glyph->bitmap.rows,
                               bitmap::COMPONENTS_RGBA,
                               bitmap::COMPONENTS_ALPHA,
                               face->face()->glyph->bitmap.buffer));  
  result->flip();  
  return result;
}
  
bool TrueTypeFont::renderGlyph(ftxt::utf32_char_t c,
                               uint32_t inSizeInPoints)
{
    bool result = false;
    GlyphPtr glyph = char2size2glyph[c][inSizeInPoints];
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

  vector<BitmapPtr> characterBitmaps;
  vector<uint32_t> glyphIndex; // because we might not use all bitmaps, need to preserve the original indices of the glyphs
  vector<GlyphPtr>::iterator i;
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
                    bitmap::COMPONENTS_RGBA, // bitmaps rendered byte the renderGlyphToBitmap method are always RGBA
                    false, // don't rotate rectbecause we don't support it yet
                    false); // don't sort because we don't want to alter position of previously rendered glyphs in texture
  
  // check if all bitmaps were packed, throw if not
  if(packerResult.rects.size() != characterBitmaps.size())
    throw runtime_error("couldn't pack all bitmaps in characterAtlas");
    
  // recreate atlas texture from current packed bitmap
  // glyphs that were rendered in a previous pass should end up in the same spot in the atlas since we forced the packer to omit sorting
  if(!atlas)
  {
    atlas.reset(new gl::Texture(packerResult.packedBitmap));
  }
  else
  {
//    DOUT("reinitialising bitmap");
    atlas->init(packerResult.packedBitmap);
  }
  
  // tell the glyphs where they are in the atlas texture in tex coords
  // the rects are reordered in the result, but the bitmapids are preserved in the vector of the same name
  for(uint32_t j=0; j<packerResult.rects.size(); ++j)
  {
    glyphs[glyphIndex[packerResult.bitmapIds[j]]]->rect = packerResult.rects[j];
  }  
}

void TrueTypeFont::addGlyph(std::vector<math::Rect>& characterRects, 
                            std::vector<math::Rect>& pixelCoordRects,
                            GlyphPtr glyph,
                            float xoffset,
                            lost::math::Vec2& pmin,
                            lost::math::Vec2& pmax)
{
  Rect tr = glyph->rect; 
  tr.x = xoffset+glyph->xoffset;
  tr.y = (float)glyph->yoffset;
  
  characterRects.push_back(tr);
  pixelCoordRects.push_back(glyph->rect);
  
  pmin.x = min(pmin.x, tr.x);
  pmin.y = min(pmin.y, tr.y);

  pmax.x = max(pmax.x, tr.maxX());
  pmax.y = max(pmax.y, tr.maxY());
}

RenderedTextPtr TrueTypeFont::render(const std::string & inText, uint32_t inSizeInPoints)
{
//  DOUT("rendering utf-8 text " << inText << " with size "<<inSizeInPoints<<" atlas size: "<<atlasSize);

  // Assume std::string is always utf8 encoded.
  ftxt::utf32_string decodedString;

  ftxt::utf8_decoder decoder;
  ftxt::decode(decoder, inText.begin(), inText.end(),
         std::back_insert_iterator<ftxt::utf32_string>(decodedString));

  return render(decodedString, inSizeInPoints);
}

void TrueTypeFont::render(const std::string & inText, uint32_t inSizeInPoints, RenderedTextPtr target)
{
//  DOUT("rendering utf-8 text " << inText << " with size "<<inSizeInPoints<<" atlas size: "<<atlasSize);

  // Assume std::string is always utf8 encoded.
  ftxt::utf32_string decodedString;

  ftxt::utf8_decoder decoder;
  ftxt::decode(decoder, inText.begin(), inText.end(),
         std::back_insert_iterator<ftxt::utf32_string>(decodedString));

  render(decodedString, inSizeInPoints, target);
}
  
void TrueTypeFont::flagDrawableChars(const ftxt::utf32_string& inText,
                                     uint32_t inSizeInPoints)
{
  for(uint32_t i=0; i<inText.size(); ++i)
  {
    shared_ptr<Glyph> glyph = char2size2glyph[inText[i]][inSizeInPoints];
    if(glyph && glyph->bitmap && (glyph->bitmap->width > 0) && (glyph->bitmap->height > 0))
    {
      glyph->drawable = true;
    }
    else
    {
      if(glyph)
        glyph->drawable = false;
    }
  }
}
  
RenderedTextPtr TrueTypeFont::render(const ftxt::utf32_string& inText,
                              uint32_t inSizeInPoints)
{

  RenderedTextPtr result(new RenderedText);
  render(inText, inSizeInPoints, result);
  return result;
}

void TrueTypeFont::render(const fhtagn::text::utf32_string& inText,
            uint32_t inSizeInPoints,
            RenderedTextPtr target)
{
//  DOUT("rendering text with size "<<inSizeInPoints<<" atlas size: "<<atlasSize);
  // these arrays will receive the character geometry in space, relative to a 0,0 baseline
  // and the corresponding pixel coordinates of the subtexture within the font texture atlas
  // used to draw the character
  std::vector<math::Rect> characterRects;
  std::vector<math::Rect> pixelCoordRects;
    
  // render glyphs if required
  uint32_t renderedGlyphs = 0;
  for(unsigned int i=0; i<inText.length(); ++i)
  {
    if(renderGlyph(inText[i], inSizeInPoints))
      ++renderedGlyphs;
  }
//  DOUT("rendered "<<renderedGlyphs<<" new glyphs");
  // rebuild atlas if any new glyphs were rendered
  if(renderedGlyphs > 0)
    rebuildTextureAtlas();

  // flag drawable characters AFTER the bitmaps have been created so we can distinguish between
  // drawables and non-darwables. This is important so we don't create geometry for characters
  // that don't need any (spaces)
  flagDrawableChars(inText, inSizeInPoints);
  float xoffset = 0;
  
  // kerning setup
  bool hasKerning = FT_HAS_KERNING(face->face()) != 0;
  FT_UInt previousGlyphIndex, currentGlyphIndex;
  previousGlyphIndex = 0;
  FT_Vector kerningDelta;
  
  // size calculation 
  Vec2 pmin, pmax; 
  uint32_t numChars = inText.size();
  uint32_t addIndex=0; // we iterate over all chracters, but not all of them might be drawable
                       // so we need a separate index for the actual insertion of a character into the mesh
  for(unsigned int i=0; i<numChars; ++i)
  {
    ftxt::utf32_char_t c = inText[i];
    
    if(hasKerning)
    {
      currentGlyphIndex = FT_Get_Char_Index(face->face(), c);
      FT_Get_Kerning(face->face(), previousGlyphIndex, currentGlyphIndex, FT_KERNING_DEFAULT, &kerningDelta);
      signed long kerningoffset = kerningDelta.x>>6;
      xoffset+=kerningoffset;
      previousGlyphIndex = currentGlyphIndex;
    }
    
    GlyphPtr glyph = char2size2glyph[c][inSizeInPoints];
    if (!glyph) continue;

    if (glyph->drawable)
    {
      addGlyph(characterRects, pixelCoordRects, glyph, xoffset, pmin, pmax);
      addIndex++;
    }
    xoffset+=glyph->advance;
  }

  target->init(characterRects, atlas, pixelCoordRects, false);
  target->min = pmin;
  target->max = pmax;
  target->size.width = (pmax.x-pmin.x)+1;  
  target->size.height = (pmax.y-pmin.y)+1;
  target->material->blend = true;
  target->material->blendSrc = GL_SRC_ALPHA;
  target->material->blendDest = GL_ONE_MINUS_SRC_ALPHA;
}


}  
}
