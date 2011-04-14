#ifndef LOST_MATH_RECT_H
#define LOST_MATH_RECT_H

#include "lost/math/forward.h"

namespace lost
{

namespace math
{

  struct Rect
  {
    Rect(float nx, float ny, float w, float h);
    Rect();

    void operator +=( const lost::math::Vec2& parentPos );
    bool operator ==(const Rect& rect) const;
    bool operator !=(const Rect& rect) const;
    bool operator !();
    lost::math::Vec2 pos() const;
    void clipTo(const lost::math::Rect& parent);
    float maxX() const;
    float maxY() const;
    float area() const;
    bool fitsInto(const Rect& inTarget) const;    
    bool contains(const lost::math::Vec2& inPoint) const;
    float left() const;
    float right() const;
    float top() const;
    float bottom() const;

    Vec2 center() const;
    bool intersects(const Rect& r2) const;
    void reset( float inX = 0, float inY = 0, float inWidth = 0, float inHeight = 0 );

    lost::math::Vec2 bottomLeft() const;
    lost::math::Vec2 bottomRight() const;
    lost::math::Vec2 topRight() const;
    lost::math::Vec2 topLeft() const;
    
    float x;
    float y;
    float width;
    float height;
  };
}
}

#endif
