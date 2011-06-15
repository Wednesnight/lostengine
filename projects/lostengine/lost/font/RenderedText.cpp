#include "lost/font/RenderedText.h"
#include "lost/common/Logger.h"
#include "lost/math/io.h"
#include "lost/profiler/Blackbox.h"

namespace lost
{
namespace font
{    

#define bb_rt_key "lost.font.RenderedText"

RenderedText::RenderedText() : mesh::Quad()
{
BB_INC(bb_rt_key);
}

RenderedText::RenderedText(const vector<math::Rect>& rects,
             gl::TexturePtr tex,
             const vector<math::Rect>& pixelCoords) : mesh::Quad(rects, tex, pixelCoords, false)
{
BB_INC(bb_rt_key);
}

RenderedText::~RenderedText()
{
BB_DEC(bb_rt_key);
}

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

void RenderedText::resetCharacterMetrics()
{
  characterMetrics.reset(new vector<vector<math::Rect> >);
}

void RenderedText::pushEmptyCharacterMetricLine()
{
  characterMetrics->push_back(vector<math::Rect>());
}

void RenderedText::pushCharacterRect(const math::Rect& cr)
{
  int32_t line = characterMetrics->size();
  line = std::max(line-1, 0);
//  DOUT("// pushing char metrics rect "<<line<<" "<<characterMetrics->at(line).size()<<" "<<cr);
  characterMetrics->at(line).push_back(cr);
}

math::Rect RenderedText::characterRect(uint32_t lineIndex, uint32_t charIndex)
{
  math::Rect result;
  
  if(characterMetrics && (lineIndex < characterMetrics->size()))
  {
    if(charIndex < characterMetrics->at(lineIndex).size())
    {
//      DOUT("cr found");
      result = characterMetrics->at(lineIndex).at(charIndex);
    }
  }
  
  return result;
}


}
}