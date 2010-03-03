#ifndef LOST_FONT_RENDEREDTEXT
#define LOST_FONT_RENDEREDTEXT

#include "lost/math/Vec2.h"
#include "lost/gl/Texture.h"
#include "lost/mesh/Quad.h"

namespace lost
{

  namespace font
  {
    struct RenderedText;
    typedef lost::shared_ptr<RenderedText> RenderedTextPtr;
    
    struct RenderedText : mesh::Quad
    {
      RenderedText() : mesh::Quad()
      {
      }
    
      RenderedText(const std::vector<math::Rect>& rects,
                   gl::TexturePtr tex,
                   const std::vector<math::Rect>& pixelCoords) : mesh::Quad(rects, tex, pixelCoords, false)
      {
      }
      
      virtual ~RenderedText() {};

      static RenderedTextPtr create() { return RenderedTextPtr(new RenderedText); };

      lost::math::Vec2 min; // min in pixel of the rendered string. baseline is at y=0
      lost::math::Vec2 max; // max in pixel of the rendered string. baseline is at y=0
      lost::math::Vec2 size; // min, max, width and height in pixel of the rendered string. baseline is at y=0
    };

  }
}

#endif
