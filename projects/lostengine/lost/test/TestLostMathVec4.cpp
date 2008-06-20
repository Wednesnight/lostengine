#include "lost/math/Vec4.h"
#include "UnitTest++.h"

using namespace lost::math;
SUITE(Vec4Test)
{
TEST(compile)
{  
  Vec4 vec;
  CHECK(&vec);
}

TEST(comparison)
{
  Vec4 v1, v2;
  CHECK(v1 == v2);
  
  v1.x = 1;
  v1.y = 2;
  v1.z = 3;
  v1.w = 4;

  v2.x = 1;
  v2.y = 2;
  v2.z = 3;
  v2.w = 4;
  CHECK(v1 == v2);
  
  v2.w = 1;
  v2.z = 2;
  v2.y = 3;
  v2.x = 4;
  CHECK(!(v1 == v2));
}

TEST(multiply)
{
  Vec4 v1(2, 1, 1, 1), v2(1, 1, 1,1);

  float v = v1*v2;
  CHECK(v == 5);
}

}
