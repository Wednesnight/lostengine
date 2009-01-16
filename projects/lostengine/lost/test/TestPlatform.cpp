#include "lost/platform/Platform.h"
#include "UnitTest++.h"

TEST(application_directory)
{
  using namespace lost::platform;
  CHECK(getApplicationDirectory() != "");
}

