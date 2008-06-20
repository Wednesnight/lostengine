#include "lost/guiro/common/Dimension.h"
#include "UnitTest++.h"

using namespace lost::guiro::common;

TEST(dimension_creation)
{
  Dimension d1;
  CHECK_EQUAL(d1.relative, 0.0f);
  CHECK_EQUAL(d1.absolute, 0.0f);

  Dimension d2(1, 2);
  CHECK_EQUAL(d2.relative, 1.0f);
  CHECK_EQUAL(d2.absolute, 2.0f);
}

TEST(basic_calculation)
{
  Dimension d1(1.0f, 0.0f);
  CHECK_EQUAL(d1(1), 1.0f);

  Dimension d2(.5f, 0.0f);
  CHECK_EQUAL(d2(1), .5f);

  Dimension d3(.5f, 10.0f);
  CHECK_EQUAL(d3(10), 15.f);
}

TEST(leftedge)
{
  lost::math::Rect r(3, 4, 11, 12);
  leftedge le(1, 0);
  float currentWidth = 11;
  float currentHeight = 12;
  CHECK_EQUAL(le(currentWidth, currentHeight, r), 14.0f);
}
