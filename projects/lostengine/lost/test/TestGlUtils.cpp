#include "lost/gl/Utils.h"
#include "lost/math/lmath.h"
#include "UnitTest++.h"

using namespace lost::gl::utils;
using namespace lost::math;

TEST(gl_utils)
{  
  CHECK(nextPowerOf2(8) == 8);
  CHECK(nextPowerOf2(9) == 16);
  CHECK(nextPowerOf2(0) == 1);
}
