#include "lost/gl/RangeManager.h"
#include "UnitTest++.h"
#include <stdexcept>

using namespace lost::gl;

TEST(rangemanager)
{ 
  RangeManager rm;
  
  CHECK_THROW(rm.alloc(2), std::runtime_error);
  
  rm.reset(10);
  
  RangeManager::RangeId r1 = rm.alloc(5);
  RangeManager::RangeId r2 = rm.alloc(3);

  CHECK(r1 == 1);
  CHECK(r2 == 2);
}