#include "lost/font/Glyph.h"
#include "lost/font/freetype/Face.h"

namespace lost
{
namespace font
{

Glyph::Glyph()
{
  drawable = true;
  xoffset = 0;
  yoffset = 0;
  advance = 0;
}

Glyph::~Glyph()
{
}

void Glyph::render(freetype::FacePtr face, uint32_t size, uint32_t c)
{
  bitmap = face->renderGlyphToBitmap(c, size);
  xoffset = face->xoffset();
  yoffset = face->yoffset();
  advance = face->advance();
}

}
}