#include "lost/math/Vec2.h"
#include "lost/math/Rect.h"

namespace lost
{
namespace math
{


    Rect::Rect(float nx, float ny, float w, float h)
    : x(nx), y(ny), width(w), height(h) {}

    Rect::Rect() : x(0), y(0), width(0), height(0) {}

    void Rect::operator +=( const lost::math::Vec2& parentPos )
    {
      x += parentPos.x;
      y += parentPos.y;
    }

    bool Rect::operator ==(const Rect& rect)
    {
      return (x == rect.x &&
              y == rect.y &&
              width == rect.width &&
              height == rect.height);
    }
    
    bool Rect::operator !=(const Rect& rect)
    {
      return !(*this == rect);
    }
    
    bool Rect::operator !()
    {
      return !(this->x || this->y || this->width || this->height);
    }
    
    lost::math::Vec2 Rect::pos() const
    {
      return lost::math::Vec2( x, y );
    }

    void Rect::clipTo(const lost::math::Rect& parent)
    {
      if(x < parent.x) { x = parent.x;}
      else if(x > parent.maxX()) {x = parent.maxX(); }

      if(y < parent.y) { y = parent.y;}
      else if(y > parent.maxY()) {y = parent.maxY(); }

      if(maxX() > parent.maxX()) { width = (parent.maxX() - x)+1.0f;}

      if(maxY() > parent.maxY()) { height = (parent.maxY() - y)+1.0f;}
    }

    float Rect::maxX() const
    {
      float result = (x + width)-1.0f;
      if(result < x) {result = x; }
      return result;
    }

    float Rect::maxY() const
    {
      float result = (y + height)-1.0f;
      if(result < y) {result = y; }
      return result;
    }

    float Rect::area() const { return width*height; } 
    
    bool Rect::fitsInto(const Rect& inTarget) const
    {
      if((width <= inTarget.width) &&(height <= inTarget.height))
        return true;
      else
        return false;
    }
    
    bool Rect::contains(const lost::math::Vec2& inPoint) const
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

    float Rect::left() const {return x;} 
    float Rect::right() const {return maxX();}
    float Rect::top() const {return maxY();}
    float Rect::bottom() const {return y;}

    Vec2 Rect::center() const
    {
      float cx = width / 2;
      float cy = height / 2;
      return Vec2(x+cx, y+cy);
    }

    bool Rect::intersects(const Rect& r2) const
    {
      return !((r2.left() > right()) ||
               (r2.right() < left()) ||
               (r2.top() < bottom()) ||
               (r2.bottom() > top()));
    }

    void Rect::reset( float inX, float inY, float inWidth, float inHeight)
    {
      x      = inX;
      y      = inY;
      width  = inWidth;
      height = inHeight;
    }

    lost::math::Vec2 Rect::bottomLeft() const { return lost::math::Vec2(x, y); }
    lost::math::Vec2 Rect::bottomRight() const { return lost::math::Vec2(maxX(), y); }
    lost::math::Vec2 Rect::topRight() const { return lost::math::Vec2(maxX(), maxY()); }
    lost::math::Vec2 Rect::topLeft() const { return lost::math::Vec2(x, maxY()); }

}
}