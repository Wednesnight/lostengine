#include "lost/guiro/common/Area.h"
#include "UnitTest++.h"

using namespace lost::guiro::common;

TEST(area_creation)
{
  Area a1; // empty area with all absolute coords
  // fully specified area with varying types of dimensions
  Area a2 = make_area(xalignleft(), yaligntop(), wabs(20), hrel(.5));
  // missing dimensions default to absolute zeros
  Area a3 = make_area(xabs(0), yabs(0));
}
