/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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
