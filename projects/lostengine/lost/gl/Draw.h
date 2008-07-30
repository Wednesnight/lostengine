#ifndef LOST_GL_DRAW_H
#define LOST_GL_DRAW_H

#include "lost/gl/gl.h"
#include "lost/common/Color.h"
#include "lost/math/Rect.h"
#include "lost/gl/Texture.h"
#include "lost/math/AABB.h"
#include "lost/math/Vec3.h"

namespace lost
{
namespace gl
{
/*      void render(const Bitmap&, float x, float y)
      {
        if (data)
        {
          glRasterPos2f(x, y);
          glDrawPixels( width, height, format, type, getPixels());
        }
      }*/

#if !defined(TARGET_IPHONE_SIMULATOR) && !defined(TARGET_IPHONE)
  // FIXME: what do we do about these glut bitmap fonts in OpenGL ES? Do we need 
  // a cut down font drawing engine with built in resources only,
  // or do we fully rely on truetype rendering?
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
#endif
  
  inline void setColor(const lost::common::Color& col)
  {
    glColor4f(col.fv[0], col.fv[1], col.fv[2], col.fv[3]); // OpenGL ES compatible
  }

  inline void drawLine(const lost::math::Vec2& start, const lost::math::Vec2& end)
  {
    float p[4];
    p[0] = start.x;
    p[1] = start.y;
    p[2] = end.x;
    p[3] = end.y;
    
    glVertexPointer(2, GL_FLOAT, 0, p);
    glDrawArrays(GL_LINES, 0,2);    
  }

  inline void drawLine(const lost::math::Vec3& start, const lost::math::Vec3& end)
  {
    float p[6];
    p[0] = start.x;
    p[1] = start.y;
    p[2] = start.z;
    p[3] = end.x;
    p[4] = end.y;
    p[5] = end.z;
    
    glVertexPointer(3, GL_FLOAT, 0, p);
    glDrawArrays(GL_LINES, 0,2);    
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
  
  inline void drawAABB(const lost::math::AABB& box)
  {
    drawLine(math::Vec3(box.origin.x, box.origin.y, box.origin.z), math::Vec3(box.origin.x + box.size.x, box.origin.y, box.origin.z));    
    drawLine(math::Vec3(box.origin.x + box.size.x, box.origin.y, box.origin.z), math::Vec3(box.origin.x + box.size.x, box.origin.y + box.size.y, box.origin.z));
    drawLine(math::Vec3(box.origin.x + box.size.x, box.origin.y + box.size.y, box.origin.z), math::Vec3(box.origin.x, box.origin.y + box.size.y, box.origin.z));    
    drawLine(math::Vec3(box.origin.x, box.origin.y + box.size.y, box.origin.z), math::Vec3(box.origin.x, box.origin.y, box.origin.z));
    drawLine(math::Vec3(box.origin.x, box.origin.y, box.origin.z),math::Vec3(box.origin.x, box.origin.y, box.origin.z + box.size.z));    
    drawLine(math::Vec3(box.origin.x, box.origin.y, box.origin.z + box.size.z), math::Vec3(box.origin.x, box.origin.y + box.size.y, box.origin.z + box.size.z));    
    drawLine(math::Vec3(box.origin.x, box.origin.y + box.size.y, box.origin.z + box.size.z), math::Vec3(box.origin.x, box.origin.y + box.size.y, box.origin.z));    
    drawLine(math::Vec3(box.origin.x, box.origin.y, box.origin.z + box.size.z), math::Vec3(box.origin.x + box.size.x, box.origin.y, box.origin.z + box.size.z));
    drawLine(math::Vec3(box.origin.x + box.size.x, box.origin.y, box.origin.z + box.size.z), math::Vec3(box.origin.x + box.size.x, box.origin.y + box.size.y, box.origin.z + box.size.z));
    drawLine(math::Vec3(box.origin.x + box.size.x, box.origin.y + box.size.y, box.origin.z + box.size.z), math::Vec3(box.origin.x, box.origin.y + box.size.y, box.origin.z + box.size.z));
    drawLine(math::Vec3(box.origin.x + box.size.x, box.origin.y, box.origin.z + box.size.z),math::Vec3(box.origin.x + box.size.x, box.origin.y, box.origin.z));
    drawLine(math::Vec3(box.origin.x + box.size.x, box.origin.y + box.size.y, box.origin.z + box.size.z),math::Vec3(box.origin.x + box.size.x, box.origin.y + box.size.y, box.origin.z));
  }
  
  inline void drawAxes(const lost::math::Vec3& length)
  {
    // Draw the positive side of the lines x,y,z
    setColor(lost::common::greenColor);         // Green for x axis
    drawLine(math::Vec3(0.0f, 0.0f, 0.0f), math::Vec3(length.x, 0.0f, 0.0f));
    setColor(lost::common::redColor);           // Red for y axis
    drawLine(math::Vec3(0.0f, 0.0f, 0.0f), math::Vec3(0.0f, length.y, 0.0f));
    setColor(lost::common::blueColor);          // Blue for z axis
    drawLine(math::Vec3(0.0f, 0.0f, 0.0f), math::Vec3(0.0f, 0.0f, length.z));
    
    // transparent lines for the negative sides of x,y,z coordinates
    // this is kinda stupid, but OpenGL ES doesn't have line stippling anymore
    float a = .15f;
    glColor4f(0,1,0,a);         // Green for x axis
    drawLine(lost::math::Vec3(-length.x, 0.0f, 0.0f), lost::math::Vec3(0.0f, 0.0f, 0.0f));
    glColor4f(1,0,0,a);           // Red for y axis
    drawLine(lost::math::Vec3(0.0f, 0.0f, 0.0f), lost::math::Vec3(0.0f, -length.y, 0.0f));
    glColor4f(0,0,1,a);          // Blue for z axis
    drawLine(lost::math::Vec3(0.0f, 0.0f, 0.0f), lost::math::Vec3(0.0f, 0.0f, -length.z));
  }
  
}
}

#endif
