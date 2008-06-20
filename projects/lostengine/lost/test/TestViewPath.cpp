#include "lost/guiro/common/ViewPath.h"
#include "UnitTest++.h"

SUITE(ViewPath)
{
  TEST(popfront_empty)
  {
    lost::guiro::common::ViewPath vpath("");
    CHECK(vpath.size() == 0);
    CHECK_THROW(vpath.popFront(), std::runtime_error);
  }
  
  TEST(construction)
  {
    lost::guiro::common::ViewPath vpath("asd/qwe");
    CHECK(vpath.size() == 2);

    vpath.reset("/asd/qwe/");
    CHECK(vpath.size() == 2);
  }
  
  TEST(traversal)
  {
    lost::guiro::common::ViewPath vpath("/////////asd////////*//////qwe//");
    CHECK(vpath.size() == 3);
    CHECK(vpath.front() == "asd");
    vpath.popFront();
    CHECK(vpath.size() == 2);
    CHECK(vpath.front() == "*");
    vpath.popFront();
    CHECK(vpath.size() == 1);
    CHECK(vpath.front() == "qwe");
    vpath.popFront();
    CHECK(vpath.size() == 0);
    CHECK_THROW(vpath.front(), std::runtime_error);    
    CHECK_THROW(vpath.popFront(), std::runtime_error);    
  }
}
