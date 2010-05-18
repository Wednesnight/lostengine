#include "lost/math/forward.h"
#include <iosfwd>

std::ostream& operator<<(std::ostream& s, const lost::math::Matrix& m);
std::ostream& operator<<(std::ostream& stream, const lost::math::Rect& rct);
std::ostream& operator<<(std::ostream& stream, const lost::math::Vec2& vec);
std::ostream& operator<<(std::ostream& s, const lost::math::Vec3& v);
std::ostream& operator << (std::ostream& stream, const lost::math::Vec4& v);
