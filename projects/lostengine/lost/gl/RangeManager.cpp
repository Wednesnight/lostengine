#include "lost/gl/rangeManager.h"

namespace lost
{
namespace gl
{

RangeManager::RangeManager()
{
  lastRangeId = 0;
}

RangeManager::~RangeManager()
{
}

RangeManager::RangeId RangeManager::alloc(uint32_t num)
{
  RangeManager::RangeId result = 0;
  
  return result;
}

void RangeManager::free(RangeId range)
{
}

void RangeManager::reset(uint32_t num)
{
  lastRangeId = 0;
  id2range.clear();
  freeCount2Id.clear();
  usedCount2Id.clear();
  
  Range r;
  r.rangeId = createRangeId();
  r.start = 0;
  r.count = num;
  r.unused = true;
  
  id2range[r.rangeId] = r;
//  freeCount2Id[r.count] = r.rangeId;
}

RangeManager::RangeId RangeManager::createRangeId()
{
  lastRangeId++;
  return lastRangeId;
}

}
}