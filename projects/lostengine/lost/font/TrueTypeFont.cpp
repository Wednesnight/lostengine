#include "lost/font/TrueTypeFont.h"
#include "lost/common/Logger.h"
#include "lost/bitmap/Packer.h"
#include "lost/gl/Texture.h"
#include <stdexcept>
#include <fhtagn/text/transcoding.h>
#include <fhtagn/text/decoders.h>
#include "lost/font/freetype/Library.h"
#include "lost/font/freetype/Face.h"
#include "lost/bitmap/Bitmap.h"
#include "lost/font/Glyph.h"
#include <math.h>

using namespace lost::bitmap;
using namespace lost::font::freetype;
using namespace lost::math;

namespace ftxt = fhtagn::text;

namespace lost
{
namespace font
{

TrueTypeFont::TrueTypeFont(freetype::LibraryPtr inLibrary,
                           common::DataPtr inData,
                           uint32_t inSizeInPoints)
{
  face.reset(new Face(inLibrary, inData,inSizeInPoints));
  size = inSizeInPoints;
  atlasSize.width = 256;
  atlasSize.height = 256;
  ascender = face->ascender();
  descender = face->descender();  
  lineHeight = face->height();
  rebuildTextureAtlas(); // so we have an initial texture and don't crash upo first text draw call
}

TrueTypeFont::~TrueTypeFont()
{
}
  
bool TrueTypeFont::renderGlyph(ftxt::utf32_char_t c)
{
    bool result = false;
    GlyphPtr glyph = char2glyph[c];
    if(!glyph) 
    {
      result = true;
      glyph.reset(new Glyph);
      glyph->render(face, size, c);
      char2glyph[c] = glyph;
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
    throw std::runtime_error("couldn't pack all bitmaps in characterAtlas");
    
  // recreate atlas texture from current packed bitmap
  // glyphs that were rendered in a previous pass should end up in the same spot in the atlas since we forced the packer to omit sorting
  if(!atlas)
  {
//    DOUT("--- CREATING FONT ATLAS");
    atlas.reset(new gl::Texture(packerResult.packedBitmap));
  }
  else
  {
//    DOUT("reinitialising bitmap");
    atlas->bind();
    atlas->init(packerResult.packedBitmap);
  }
  
  // tell the glyphs where they are in the atlas texture in tex coords
  // the rects are reordered in the result, but the bitmapids are preserved in the vector of the same name
  for(uint32_t j=0; j<packerResult.rects.size(); ++j)
  {
    glyphs[glyphIndex[packerResult.bitmapIds[j]]]->rect = packerResult.rects[j];
  }  
}
    
GlyphPtr TrueTypeFont::glyph(uint32_t utf32character)
{
  GlyphPtr result = char2glyph[utf32character];
  if(!result)
  {
    if(renderGlyph(utf32character))
    {
      rebuildTextureAtlas();
    }
    result = char2glyph[utf32character];
    if(result && result->bitmap && (result->bitmap->width > 0) && (result->bitmap->height > 0))
    {
      result->drawable = true;
    }
    else
    {
      if(result)
        result->drawable = false;
    }
  }
  return result;
}

float TrueTypeFont::kerningOffset(uint32_t previousChar, uint32_t currentChar)
{
  float result = 0.0f;
  if(face->hasKerning()) {
    face->kerningOffset(previousChar, currentChar);
  }
  return result;
}

bool TrueTypeFont::hasKerning()
{
  return face->hasKerning();
}

float TrueTypeFont::characterAdvance(uint32_t previousChar, uint32_t currentChar)
{
  float result = 0.0f;
  GlyphPtr g = this->glyph(currentChar);
  if(g)
  {
    result = g->advance + kerningOffset(previousChar, currentChar);
  }
  return result;
}

}  
}
