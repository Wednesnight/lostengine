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
