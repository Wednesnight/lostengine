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

#ifndef LOST_FONT_RENDEREDTEXT
#define LOST_FONT_RENDEREDTEXT

#include "lost/math/Vec2.h"
#include "lost/gl/forward.h"
#include "lost/mesh/Quad.h"
#include "lost/font/forward.h"

namespace lost
{
namespace font
{    

struct RenderedText : mesh::Quad
{
  RenderedText();
  RenderedText(const vector<math::Rect>& rects,
               gl::TexturePtr tex,
               const vector<math::Rect>& pixelCoords);
  virtual ~RenderedText();
  static RenderedTextPtr create();
  mesh::MeshPtr clone();

  lost::math::Vec2 min; // min in pixel of the rendered string. baseline is at y=0
  lost::math::Vec2 max; // max in pixel of the rendered string. baseline is at y=0
  lost::math::Vec2 size; // min, max, width and height in pixel of the rendered string. baseline is at y=0
  float numLines;
  
  void resetCharacterMetrics(); // resets the shared_ptr, clearing all data and adding a new empty vector
  void pushEmptyCharacterMetricLine(); // pushes a new empty line vector
  void pushCharacterRect(const math::Rect& cr); // pushes a character rect into the last line
  math::Rect characterRect(uint32_t lineIndex, uint32_t charIndex); // returns the rect of the character at the specified position. 0 will be returned if any index is out of range.
  
  lost::shared_ptr<vector<vector<math::Rect> > > characterMetrics;
};

}
}

#endif
