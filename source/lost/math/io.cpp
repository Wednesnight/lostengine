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

#include "lost/math/Matrix.h"
#include "lost/math/Rect.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec4.h"
#include <iostream>

std::ostream& operator<<(std::ostream& s, const lost::math::Matrix& m)
{
  s << m.m[0] << " " << m.m[4] << " " << m.m[8] << " " << m.m[12] << std::endl;
  s << m.m[1] << " " << m.m[5] << " " << m.m[9] << " "  << m.m[13] << std::endl;
  s << m.m[2] << " " << m.m[6] << " " << m.m[10] << " " << m.m[14] << std::endl;
  s << m.m[3] << " " << m.m[7] << " " << m.m[11] << " " << m.m[15] << std::endl;
  return s;
}

std::ostream& operator<<(std::ostream& stream, const lost::math::Rect& rct)
{
  stream << rct.x << " " << rct.y << " " << rct.width << " " << rct.height;
  return stream;
}

std::ostream& operator<<(std::ostream& stream, const lost::math::Vec2& vec)
{
  stream << vec.x << " " << vec.y;
  return stream;
}

std::ostream& operator<<(std::ostream& s, const lost::math::Vec3& v)
{
    s << v.x << " " << v.y << " "<< v.z;
    return s;
}

std::ostream& operator << (std::ostream& stream, const lost::math::Vec4& v)
{
  stream << "(" << v.x << "/" << v.y << "/" << v.z << "/" << v.w << ")";
  return stream;
}
