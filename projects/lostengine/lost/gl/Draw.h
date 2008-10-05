#ifndef LOST_GL_DRAW_H
#define LOST_GL_DRAW_H

#include "lost/gl/gl.h"
#include "lost/gl/Texture.h"

namespace lost
{

namespace math{ struct Vec3; struct Vec2;  struct AABB; struct Rect;}
namespace common{ struct Color; };

namespace gl
{
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
  void drawString(int x, int y, const std::string& text, void* font);
#endif
  
  void setColor(const lost::common::Color& col);
  void drawLine(const lost::math::Vec2& start, const lost::math::Vec2& end);
  void drawPoint(const lost::math::Vec2& point);
  void drawLine(const lost::math::Vec3& start, const lost::math::Vec3& end);  
  void drawRectOutline(const lost::math::Rect& rect);
  void drawRectFilled(const lost::math::Rect& rect);
  // don't forget glEnable(GL_TEXTURE_2D);
  // rect is drawn counterclockwise
  // requires vertex arrays, texture arrays, index arrays
  // FIXME: needs serious optimisation/rethinking, but at least it works
  void drawRectTextured(const lost::math::Rect& rect,
                               boost::shared_ptr<const lost::gl::Texture> tex,
                               bool flip=true);
  void drawAABB(const lost::math::AABB& box);
  void drawAxes(const lost::math::Vec3& length);  
}
}

#endif
