#ifndef LOST_GL_DRAW_H
#define LOST_GL_DRAW_H

#include "lost/gl/gl.h"
#include "lost/common/Color.h"
#include "lost/math/Rect.h"
#include "lost/gl/Texture.h"

namespace lost
{
namespace gl
{
  /** draw a string at the specified position with a builtin glut font
   * font can be one of the following defines:
   *  GLUT_BITMAP_9_BY_15
   *  GLUT_BITMAP_8_BY_13
   *  GLUT_BITMAP_TIMES_ROMAN_10
   *  GLUT_BITMAP_TIMES_ROMAN_24
   *  GLUT_BITMAP_HELVETICA_10
   *  GLUT_BITMAP_HELVETICA_12
   *  GLUT_BITMAP_HELVETICA_18
   */
  inline void drawString(int x, int y, const std::string& text, void* font)
  {
    int len, i;
    glRasterPos2f(x, y);
    len = text.size();
    for (i = 0; i < len; i++)
    {
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text[i]);
    }
  }

  inline void setColor(const lost::common::Color& col)
  {
    glColor4fv(col.fv);
  }

  inline void drawLine(const lost::math::Vec2& start, const lost::math::Vec2& end)
  {
    glBegin(GL_LINES);
    glVertex2f(start.x,start.y);
    glVertex2f(end.x, end.y);
    glEnd();
  }

  inline void drawRectOutline(const lost::math::Rect& rect)
  {
    drawLine(rect.bottomLeft(), rect.bottomRight());
    drawLine(rect.bottomRight(), rect.topRight());
    drawLine(rect.topRight(), rect.topLeft());
    drawLine(rect.topLeft(), rect.bottomLeft());
  }

  inline void drawRectFilled(const lost::math::Rect& rect)
  {
    glBegin(GL_QUADS);
    glVertex2f(rect.x, rect.y);
    glVertex2f(rect.x+rect.width-1, rect.y);
    glVertex2f(rect.x+rect.width-1, rect.y+rect.height-1);
    glVertex2f(rect.x, rect.y+rect.height-1);
    glEnd();
  }
  
  // don't forget glEnable(GL_TEXTURE_2D);
  // rect is drawn counterclockwise
  inline void drawRectTextured(const lost::math::Rect& rect, const lost::gl::Texture& tex, const lost::common::Color& col = lost::common::whiteColor, bool flip=true)
  {
    setColor(col);
    tex.bind();
    glBegin(GL_QUADS);
    if(!flip)
    {
      glTexCoord2f(0, 0);glVertex2f(rect.x, rect.y);
      glTexCoord2f(1, 0);glVertex2f(rect.x+rect.width-1, rect.y);
      glTexCoord2f(1, 1);glVertex2f(rect.x+rect.width-1, rect.y+rect.height-1);
      glTexCoord2f(0, 1);glVertex2f(rect.x, rect.y+rect.height-1);
    }
    else
    {
      glTexCoord2f(0, 1);glVertex2f(rect.x, rect.y);
      glTexCoord2f(1, 1);glVertex2f(rect.x+rect.width-1, rect.y);
      glTexCoord2f(1, 0);glVertex2f(rect.x+rect.width-1, rect.y+rect.height-1);
      glTexCoord2f(0, 0);glVertex2f(rect.x, rect.y+rect.height-1);    
    }
    glEnd();
  }  
}
}

#endif
