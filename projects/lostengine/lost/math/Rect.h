#ifndef LOST_MATH_RECT_H
#define LOST_MATH_RECT_H

#include "lost/math/Vec2.h"
#include <iostream>

namespace lost
{

namespace math
{

  struct Rect
  {
    Rect(float nx, float ny, float w, float h)
    : x(nx), y(ny), width(w), height(h) {}

    Rect() : x(0), y(0), width(0), height(0) {}

    void operator +=( const lost::math::Vec2& parentPos )
    {
      x += parentPos.x;
      y += parentPos.y;
    }

    lost::math::Vec2 pos() const
    {
      return lost::math::Vec2( x, y );
    }

    void clipTo(const lost::math::Rect& parent)
    {
      if(x < parent.x) { x = parent.x;}
      else if(x > parent.maxX()) {x = parent.maxX(); }

      if(y < parent.y) { y = parent.y;}
      else if(y > parent.maxY()) {y = parent.maxY(); }

      if(maxX() > parent.maxX()) { width = (parent.maxX() - x)+1.0f;}

      if(maxY() > parent.maxY()) { height = (parent.maxY() - y)+1.0f;}
    }

    float maxX() const
    {
      float result = (x + width)-1.0f;
      if(result < x) {result = x; }
      return result;
    }

    float maxY() const
    {
      float result = (y + height)-1.0f;
      if(result < y) {result = y; }
      return result;
    }

    bool contains(const lost::math::Vec2& inPoint) const
    {
      bool result = false;
      if(   (inPoint.x >= x)
         && (inPoint.x < (x+width))
         && (inPoint.y >= y)
         && (inPoint.y < (y+height))
         )
      {
        result = true;
      }
      return result;
    }

    void reset( float inX = 0, float inY = 0, float inWidth = 0, float inHeight = 0 )
    {
      x      = inX;
      y      = inY;
      width  = inWidth;
      height = inHeight;
    }

    lost::math::Vec2 bottomLeft() const { return lost::math::Vec2(x, y); }
    lost::math::Vec2 bottomRight() const { return lost::math::Vec2(x+width-1, y); }
    lost::math::Vec2 topRight() const { return lost::math::Vec2(x+width-1, y+height-1); }
    lost::math::Vec2 topLeft() const { return lost::math::Vec2(x, y+height-1); }
    
    float x;
    float y;
    float width;
    float height;
  };

  inline std::ostream& operator<<(std::ostream& stream, const lost::math::Rect& rct)
  {
    stream << rct.x << " " << rct.y << " " << rct.width << " " << rct.height;
    return stream;
  }


}

}

#endif
