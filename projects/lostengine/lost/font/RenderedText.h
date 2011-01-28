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
  RenderedText(const std::vector<math::Rect>& rects,
               gl::TexturePtr tex,
               const std::vector<math::Rect>& pixelCoords);
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
  
  lost::shared_ptr<std::vector<std::vector<math::Rect> > > characterMetrics;
};

}
}

#endif
