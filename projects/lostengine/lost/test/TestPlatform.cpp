#include "lost/platform/Platform.h"
#include "UnitTest++.h"

TEST(resource_directory)
{
  using namespace lost::platform;
  CHECK(getResourcePath() != "");
}

