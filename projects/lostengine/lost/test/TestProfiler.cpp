#include "lost/common/Profiler.h"
#include "UnitTest++.h"

struct Tester
{
  void test1()
  {
    PROFILE_METHOD()
  }

  void test2(int x, const std::string& y)
  {
    PROFILE_METHOD()
  }
};

TEST(profiler)
{
  PROFILE_METHOD()
  Tester tester;

  for (int i = 0; i < 789103; ++i)
  {
    tester.test1();
  }
  for (int i = 0; i < 678956; ++i)
  {
    tester.test2(10,"test2");
  }
}
