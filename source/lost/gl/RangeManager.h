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

#ifndef LOST_GL_RANGEMANAGER_H
#define LOST_GL_RANGEMANAGER_H

#include <iostream>

namespace lost
{
namespace gl
{

struct RangeManager
{
  typedef uint32_t RangeId;

  struct Range
  {
    RangeId   rangeId;
    uint32_t  start;
    uint32_t  count; 
    bool      unused;
    
    Range() : rangeId(0), start(0), count(0), unused(true) {}
  };

  RangeManager();
  virtual ~RangeManager();
  void reset(uint32_t num); // throws away all previous allocations, creating one empty range with num elements
  
  RangeId alloc(uint32_t num);
  void free(RangeId range);
  
  
  RangeId createRangeId();  
  RangeId           lastRangeId;
  map<RangeId, Range> id2range; // simply stores range objects by mapping id to Range
  std::multimap<uint32_t, RangeId> freeCount2Id; // free ranges, indexed by count
  std::multimap<uint32_t, RangeId> usedCount2Id; // used ranges, indexed by count
};

inline std::ostream& operator<<(std::ostream& os, const RangeManager::Range& r)
{
  os << "(" << r.rangeId << " / " << r.start << " / " << r.count << " / " << (r.unused ? "unused" : "USED") << ")";
  return os;
}

}
}

#endif

