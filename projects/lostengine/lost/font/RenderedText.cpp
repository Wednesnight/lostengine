#include "lost/font/RenderedText.h"

namespace lost
{
namespace font
{    

RenderedText::RenderedText() : mesh::Quad()
{
}

RenderedText::RenderedText(const std::vector<math::Rect>& rects,
             gl::TexturePtr tex,
             const std::vector<math::Rect>& pixelCoords) : mesh::Quad(rects, tex, pixelCoords, false)
{
}

RenderedText::~RenderedText() {}

RenderedTextPtr RenderedText::create() { return RenderedTextPtr(new RenderedText); }

mesh::MeshPtr RenderedText::clone()
{
  RenderedTextPtr result(new RenderedText);
  
  result->material = material;
  result->transform = transform;
  result->vertexBuffer = vertexBuffer;
  result->indexBuffer = indexBuffer;
  result->min = min;
  result->max = max;
  result->size = size;
  result->numLines = numLines;
  
  return result;
}

}
}