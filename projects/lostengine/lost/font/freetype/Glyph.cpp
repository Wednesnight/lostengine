#include "lost/font/freetype/Glyph.h"

namespace lost
{
namespace font
{
namespace freetype
{

Glyph::Glyph(boost::shared_ptr<lost::gl::Texture>  tex,
                                            const lost::math::Rect& rect,
                                            float maxu,
                                            float maxv,
                                            signed long inXOffset,
                                            signed long inYOffset,
                                            signed long inAdvance)
: mTexture(tex), mRect(rect), mMaxU(maxu), mMaxV(maxv), mXOffset(inXOffset), mYOffset(inYOffset), mAdvance(inAdvance)
{
}

Glyph::~Glyph()
{
}

void Glyph::draw(float xoffset, float yoffset)
{
  mTexture->bind();GLDEBUG;
  lost::math::Rect temprect = mRect;
  temprect.x += xoffset;
  temprect.y += yoffset;
  glBegin(GL_QUADS);
  glTexCoord2f(0,mMaxV);glVertex2f(temprect.x, temprect.y);
  glTexCoord2f(mMaxU,mMaxV);glVertex2f(temprect.x+temprect.width, temprect.y);
  glTexCoord2f(mMaxU,0);glVertex2f(temprect.x+temprect.width, temprect.y+temprect.height);
  glTexCoord2f(0,0);glVertex2f(temprect.x, temprect.y+temprect.height);
  glEnd();GLDEBUG;
}

}
}
}