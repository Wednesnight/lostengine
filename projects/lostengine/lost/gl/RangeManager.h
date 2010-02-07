#ifndef LOST_GL_RANGEMANAGER_H
#define LOST_GL_RANGEMANAGER_H

#include <list>
#include <map>
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
  std::map<RangeId, Range> id2range; // simply stores range objects by mapping id to Range
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

