#include "lost/math/AABB.h"

namespace lost
{
namespace math
{
AABB::AABB(Vec3 inOrigin, Vec3 inSize)
      : origin(inOrigin),
        size(inSize)
      {
      }

AABB::AABB()
      {
        AABB(Vec3(0,0,0), Vec3(0,0,0));
      }

}
}