/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/font/Render.h"
#include "lost/font/Font.h"
#include "lost/font/Glyph.h"
#include "lost/font/RenderedText.h"
#include "lost/math/Vec2.h"
#include "lost/math/Rect.h"
#include "utf8.h"
#include <math.h>

namespace lost
{
namespace font
{

using lost::math::Vec2;
using lost::math::Rect;

void addGlyph(vector<math::Rect>& characterRects, 
                            vector<math::Rect>& pixelCoordRects,
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
  
  pmin.x = std::min(pmin.x, tr.x);
  pmin.y = std::min(pmin.y, tr.y);

  pmax.x = std::max(pmax.x, tr.maxX());
  pmax.y = std::max(pmax.y, tr.maxY());
}


RenderedTextPtr render(const string & inText, const FontPtr& font, bool characterMetrics, int align)
{
  RenderedTextPtr result(new RenderedText);
  render(inText, font, result, characterMetrics);
  return result;
}

void render(const string & inUtf8String, const FontPtr& font, const RenderedTextPtr& target, bool characterMetrics, int align)
{
  text::utf32_string txt;
  utf8::utf8to32(inUtf8String.begin(), inUtf8String.end(), back_inserter(txt));
  render(txt, font, target, characterMetrics);
}

RenderedTextPtr render(const text::utf32_string& inText, const FontPtr& font, bool characterMetrics, int align)
{
  RenderedTextPtr result(new RenderedText);
  render(inText, font, result, characterMetrics);
  return result;  
}

void render(const text::utf32_string& inText, const FontPtr& font, const RenderedTextPtr& target, bool characterMetrics, int align)
{
  render(inText, Range(0, inText.size()), font, target, characterMetrics);
}

void render(const text::utf32_string& inText, const Range& range, const FontPtr& font, const RenderedTextPtr& target, bool characterMetrics, int align)
{
  vector<Range> lines;
  lines.push_back(range);
  render(inText, lines, font, target, characterMetrics);
}

void render(const text::utf32_string& inText, const vector<Range>& lines, const FontPtr& font, const RenderedTextPtr& target, bool characterMetrics, int align)
{
  // these arrays will receive the character geometry in space, relative to a 0,0 baseline
  // and the corresponding pixel coordinates of the subtexture within the font texture atlas
  // used to draw the character
  vector<math::Vec2> lineBounds;
  vector<math::Rect> characterRects;
  vector<math::Rect> pixelCoordRects;
    
  // size calculation 
  Vec2 pmin, pmax; 
  uint32_t addIndex=0; // we iterate over all chracters, but not all of them might be drawable
                       // so we need a separate index for the actual insertion of a character into the mesh

  uint32_t previousGlyphIndex = 0;
  float xoffset = 0;    
  float yoffset = std::max((float)lines.size()-1, 0.0f)*floorf(font->lineHeight);
  if(characterMetrics) {target->resetCharacterMetrics(); }
  for(vector<Range>::const_iterator pos=lines.begin(); pos!=lines.end(); ++pos)
  {
    if(characterMetrics) {target->pushEmptyCharacterMetricLine();}
    if(pos->begin != pos->end) // skip empty lines
    {
      Vec2 bounds(addIndex,addIndex);
      for(uint32_t i=pos->begin; i<pos->end; ++i)
      {
        text::utf32_char c = inText[i];
        xoffset+=font->kerningOffset(previousGlyphIndex, c);
        GlyphPtr glyph = font->glyph(c);
        if (!glyph) continue;
        if (glyph->drawable)
        {
          addGlyph(characterRects, pixelCoordRects, glyph, xoffset, yoffset, pmin, pmax);
          if(characterMetrics) {target->pushCharacterRect(characterRects[addIndex]);}
          addIndex++;
        }
        else
        {
          // push dummy rect for metrics if char was not drawable
          if(characterMetrics) {target->pushCharacterRect(Rect(xoffset,yoffset,glyph->advance,font->lineHeight));}
        }
        xoffset+=glyph->advance;
      }
      if (align != 0 && addIndex > 0) {
        bounds.max = addIndex-1;
        lineBounds.push_back(bounds);
      }
    }
    xoffset = 0;
    yoffset -= floorf(font->lineHeight);
    previousGlyphIndex = 0;
  }

  // horizontal align center/right
  if (align != 0) {
    float maxWidth = (pmax.x-pmin.x)+1;
    for (vector<math::Vec2>::iterator b = lineBounds.begin(); b != lineBounds.end(); ++b) {
      int offset = (maxWidth - (characterRects[b->max].x+characterRects[b->max].width-characterRects[b->min].x));
      if (offset > 0) {
        if (align == 1) {
          offset /= 2.0f;
        }
        for (int c = b->min; c <= b->max; ++c) {
          characterRects[c].x += offset;
        }
      }
    }
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
