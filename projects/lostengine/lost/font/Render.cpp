#include "lost/font/Render.h"
#include "lost/font/Font.h"
#include "lost/font/Glyph.h"
#include "lost/font/RenderedText.h"
#include "lost/math/Vec2.h"
#include "lost/math/Rect.h"
#include <fhtagn/text/transcoding.h>
#include <fhtagn/text/decoders.h>
#include <math.h>
//#include <boost/algorithm/string.hpp>

namespace ftxt = fhtagn::text;

namespace lost
{
namespace font
{

using lost::math::Vec2;
using lost::math::Rect;
using namespace std;

/*// splits newlines
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
void split(BreakMode mode, float width, const std::string& inText, std::vector<fhtagn::text::utf32_string>& outSplit)
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
*/
void addGlyph(std::vector<math::Rect>& characterRects, 
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


RenderedTextPtr render(const std::string & inText, const FontPtr& font)
{
  RenderedTextPtr result(new RenderedText);
  render(inText, font, result);
  return result;
}

void render(const std::string & inUtf8String, const FontPtr& font, const RenderedTextPtr& target)
{
  ftxt::utf8_decoder decoder;
  ftxt::utf32_string txt;
  ftxt::decode(decoder, inUtf8String.begin(), inUtf8String.end(),
         std::back_insert_iterator<ftxt::utf32_string>(txt));

  render(txt, font, target);
}

RenderedTextPtr render(const fhtagn::text::utf32_string& inText, const FontPtr& font)
{
  RenderedTextPtr result(new RenderedText);
  render(inText, font, result);
  return result;  
}

void render(const ftxt::utf32_string& inText, const FontPtr& font, const RenderedTextPtr& target)
{
  render(inText, Range(0, inText.size()), font, target);
}

void render(const fhtagn::text::utf32_string& inText, const Range& range, const FontPtr& font, const RenderedTextPtr& target)
{
  std::vector<Range> lines;
  lines.push_back(range);
  render(inText, lines, font, target);
}

void render(const fhtagn::text::utf32_string& inText, const std::vector<Range>& lines, const FontPtr& font, const RenderedTextPtr& target)
{
//  DOUT("rendering text with size "<<inSizeInPoints<<" atlas size: "<<atlasSize);
  // these arrays will receive the character geometry in space, relative to a 0,0 baseline
  // and the corresponding pixel coordinates of the subtexture within the font texture atlas
  // used to draw the character
  std::vector<math::Rect> characterRects;
  std::vector<math::Rect> pixelCoordRects;
  
  // kerning setup
  bool hasKerning = font->hasKerning();
  
  // size calculation 
  Vec2 pmin, pmax; 
  uint32_t addIndex=0; // we iterate over all chracters, but not all of them might be drawable
                       // so we need a separate index for the actual insertion of a character into the mesh

  uint32_t previousGlyphIndex = 0;
  font->prepareGlyphs(inText);
  float xoffset = 0;    
  float yoffset = max((float)lines.size()-1, 0.0f)*font->lineHeight;
  for(std::vector<Range>::const_iterator pos=lines.begin(); pos!=lines.end(); ++pos)
  {
    if(pos->begin != pos->end) // skip empty lines
    {
      for(uint32_t i=pos->begin; i!=pos->end; ++i)
      {
        ftxt::utf32_char_t c = inText[i];
        if(hasKerning) { xoffset+=font->kerningOffset(previousGlyphIndex, c);}
        GlyphPtr glyph = font->glyph(c);
        if (!glyph) continue;
        if (glyph->drawable)
        {
          addGlyph(characterRects, pixelCoordRects, glyph, xoffset, yoffset, pmin, pmax);
          addIndex++;
        }
        xoffset+=glyph->advance;
      }
    }
    xoffset = 0;
    yoffset -= font->lineHeight;
    previousGlyphIndex = 0;
  }

  target->init(characterRects, font->atlas, pixelCoordRects, false);
  target->min = pmin;
  target->max = pmax;
  target->size.width = (pmax.x-pmin.x)+1;  
  target->size.height = (pmax.y-pmin.y)+1;
  target->material->blendPremultiplied();
  target->numLines = lines.size();
}

}
}
