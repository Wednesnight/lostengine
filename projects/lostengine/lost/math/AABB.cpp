#include "lost/math/AABB.h"

namespace lost
{
namespace math
{
AABB::AABB(Vec3 inOrigin, Vec3 inSize)
: pos(inOrigin),
  size(inSize)
{
}

AABB::AABB()
{
  AABB(Vec3(0,0,0), Vec3(0,0,0));
}

// realtime collision detection, p. 79
bool intersect(const AABB& a, const AABB& b)
{
    float t;
    if((t=a.pos.x-b.pos.x) > b.size.x || -t > a.size.x) return false;
    if((t=a.pos.y-b.pos.y) > b.size.y || -t > a.size.y) return false;
    if((t=a.pos.z-b.pos.z) > b.size.z || -t > a.size.z) return false;
    return true;
}

}
}