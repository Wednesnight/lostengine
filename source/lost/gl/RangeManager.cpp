/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/gl/RangeManager.h"

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
