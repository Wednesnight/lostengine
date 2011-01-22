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
#include "lost/font/Glyph.h"
#include <math.h>
#include <boost/algorithm/string.hpp>

using namespace std;
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
  _size = inSizeInPoints;
  atlasSize.width = 256;
  atlasSize.height = 256;
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
      glyph->render(face, _size, c);
      char2glyph[c] = glyph;
      glyphs.push_back(glyph);

      // due to the fact that font size is not fixed within a font instance, we have to recalculate 
      ascender = face->ascender();
      descender = face->descender();
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

void TrueTypeFont::addGlyph(std::vector<math::Rect>& characterRects, 
                            std::vector<math::Rect>& pixelCoordRects,
                            GlyphPtr glyph,
                            float xoffset,
                            float yoffset,                            
                            lost::math::Vec2& pmin,
                            lost::math::Vec2& pmax)
{
  Rect tr = glyph->rect; 
  tr.x = xoffset+glyph->xoffset;
  tr.y = yoffset+glyph->yoffset;
  
  characterRects.push_back(tr);
  pixelCoordRects.push_back(glyph->rect);
  
  pmin.x = min(pmin.x, tr.x);
  pmin.y = min(pmin.y, tr.y);

  pmax.x = max(pmax.x, tr.maxX());
  pmax.y = max(pmax.y, tr.maxY());
}

RenderedTextPtr TrueTypeFont::render(const std::string & inText)
{
  RenderedTextPtr result(new RenderedText);
  render(inText, result);
  return result;
}

void TrueTypeFont::render(const std::string & inText, const RenderedTextPtr& target)
{
  vector<fhtagn::text::utf32_string> spl;
  split(BREAKMODE_NONE, 0,inText, spl);
  render(spl, target);
}
  
void TrueTypeFont::flagDrawableChars(const ftxt::utf32_string& inText)
{
  for(uint32_t i=0; i<inText.size(); ++i)
  {
    shared_ptr<Glyph> glyph = char2glyph[inText[i]];
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
  
// splits newlines
// converts result strings to utf32
void splitAtNewline(const std::string& inText, vector<fhtagn::text::utf32_string>& outSplit)
{
  std::string txt(inText); // make a copy we can modify
  // replace all newlines so only \n is left so we can safely split newlines
  vector<string> tmp;
  boost::algorithm::ireplace_all(txt, "\r\n", "\n");
  boost::algorithm::ireplace_all(txt, "\r", "\n");
  boost::algorithm::split(tmp, txt, boost::algorithm::is_any_of("\n"));

  vector<string>::iterator pos;
//  DOUT("split ---------------");
  ftxt::utf8_decoder decoder;
  for(pos=tmp.begin(); pos!=tmp.end(); ++pos)
  {
//    DOUT("'"<<*pos<<"'");

    ftxt::utf32_string decodedString;
    ftxt::decode(decoder, (*pos).begin(), (*pos).end(),
           std::back_insert_iterator<ftxt::utf32_string>(decodedString));
    outSplit.push_back(decodedString);
  }
}

// 1.) normalise newlines and split text there
// 2.) perform additional splits if char or word is specified
void TrueTypeFont::split(BreakMode mode, float width, const std::string& inText, std::vector<fhtagn::text::utf32_string>& outSplit)
{
  outSplit.clear();
  splitAtNewline(inText, outSplit);
  switch(mode)
  {
    case BREAKMODE_NONE:
    {
      break;
    }
    case BREAKMODE_CHAR:
    {
      
      break;
    }
    case BREAKMODE_WORD:
    {
      break;
    }
  }
}  
  
void TrueTypeFont::render(const vector<fhtagn::text::utf32_string>& inLines, const RenderedTextPtr& target)
{
//  DOUT("rendering text with size "<<inSizeInPoints<<" atlas size: "<<atlasSize);
  // these arrays will receive the character geometry in space, relative to a 0,0 baseline
  // and the corresponding pixel coordinates of the subtexture within the font texture atlas
  // used to draw the character
  std::vector<math::Rect> characterRects;
  std::vector<math::Rect> pixelCoordRects;
    
  
  // kerning setup
  bool hasKerning = face->hasKerning();
  
  // size calculation 
  Vec2 pmin, pmax; 
  uint32_t addIndex=0; // we iterate over all chracters, but not all of them might be drawable
                       // so we need a separate index for the actual insertion of a character into the mesh
  
  vector<fhtagn::text::utf32_string>::const_iterator pos;
  float yoffset = max(0.0f, ((float)inLines.size())-1)*floorf(face->height());// floor to prevent offcenter pixels in 2D
  for(pos=inLines.begin(); pos!=inLines.end(); ++pos)
  {
    uint32_t numChars = (*pos).size();
    uint32_t previousGlyphIndex = 0;
    prepareGlyphs(*pos);
    float xoffset = 0;    
    for(unsigned int i=0; i<numChars; ++i)
    {
      ftxt::utf32_char_t c = (*pos)[i];
      
      if(hasKerning)
      {
        xoffset+=face->kerningOffset(previousGlyphIndex, c);
      }
      
      GlyphPtr glyph = char2glyph[c];
      if (!glyph) continue;

      if (glyph->drawable)
      {
        addGlyph(characterRects, pixelCoordRects, glyph, xoffset, yoffset, pmin, pmax);
        addIndex++;
      }
      xoffset+=glyph->advance;
    }
    yoffset-=floorf(face->height()); // floor to prevent offcenter pixels in 2D
  }

  target->init(characterRects, atlas, pixelCoordRects, false);
  target->min = pmin;
  target->max = pmax;
  target->size.width = (pmax.x-pmin.x)+1;  
  target->size.height = (pmax.y-pmin.y)+1;
  target->material->blend = true;
  target->material->blendSrc = GL_SRC_ALPHA;
  target->material->blendDest = GL_ONE_MINUS_SRC_ALPHA;
  target->fontAscender = ascender;
  target->fontDescender = descender;
  target->numLines = inLines.size();
  target->lineHeight = floorf(face->height());// floor to prevent offcenter pixels in 2D
  float ascsum = fabs(ascender)+fabs(descender);
  int iascsum = floorf(ascsum+.5f);
/*  DOUT("-- rendered size: w:"<<target->size.width
     <<" height:"<<target->size.height
     <<" rounded:"<<iascsum
     <<" delta:"<<abs(target->size.height-iascsum));*/
  target->fontHeight = iascsum;
  target->pointSize = _size;
}

// for splitting with breakmode
bool isWhiteSpace(char c)
{
  bool result = false;
  switch(c)
  {
    case ' ':
    case '\t':
    case '\r':
    case '\n':
      result=true;
    default:
      result=false;
  }
  return result;
}

void TrueTypeFont::prepareGlyphs(const fhtagn::text::utf32_string& inText)
{
  // render glyphs if required
  uint32_t renderedGlyphs = 0;
  for(unsigned int i=0; i<inText.length(); ++i)
  {
    if(renderGlyph(inText[i]))
      ++renderedGlyphs;
  }
//  DOUT("rendered "<<renderedGlyphs<<" new glyphs");
  // rebuild atlas if any new glyphs were rendered
  if(renderedGlyphs > 0)
    rebuildTextureAtlas();  

  // flag drawable characters AFTER the bitmaps have been created so we can distinguish between
  // drawables and non-darwables. This is important so we don't create geometry for characters
  // that don't need any (spaces)
  flagDrawableChars(inText);
}

}  
}
