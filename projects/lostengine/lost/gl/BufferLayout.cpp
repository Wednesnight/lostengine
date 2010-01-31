#include "lost/gl/BufferLayout.h"
#include <sstream>
#include <stdexcept>

namespace lost
{
namespace gl
{

using namespace std;

uint32_t elementSize(ElementType t)
{
  uint32_t result = 0;
  switch (t) {
    case ET_u8:result=1;break;
    case ET_u16:result=2;break;
    case ET_u32:result=4;break;
    case ET_f32:result=4;break;
    case ET_vec2_u8:result=2;break;
    case ET_vec2_u16:result=4;break;
    case ET_vec2_u32:result=8;break;
    case ET_vec2_f32:result=8;break;
    case ET_vec3_u8:result=3;break;
    case ET_vec3_u16:result=6;break;
    case ET_vec3_u32:result=12;break;
    case ET_vec3_f32:result=12;break;
    case ET_vec4_u8:result=4;break;
    case ET_vec4_u16:result=8;break;
    case ET_vec4_u32:result=16;break;
    case ET_vec4_f32:result=16;break;
    case ET_mat4x4_f32:result=64;break;
    
    default:
      ostringstream os;
      os<<"can't find size for ElementType "<<t;
      throw std::runtime_error(os.str());
      break;
  }
  return result;
}

void BufferLayout::add(ElementType et, UsageType ut, uint32_t p)
{
  attributes.push_back(BufferLayoutAttribute(et, ut, p));
  partitions[p] = true;
  ut2pid[ut] = p;
  ut2et[ut] = et;
}

// number of partitions in this layout
uint32_t BufferLayout::numPartitions() const
{
  return partitions.size();
}

// size of a particular partition in bytes
uint32_t BufferLayout::partitionSize(uint32_t partitionId) const
{
  uint32_t result = 0;
  
  for(uint32_t i=0; i<attributes.size(); ++i)
  {
    if(attributes[i].partition==partitionId)
    {
      result += attributes[i].size();
    }
  }
  
  return result;
}
 
// size of the whole layout in bytes, without taking partitioning into account, in bytes
uint32_t BufferLayout::size() const
{
  uint32_t result = 0;
  for(uint32_t i=0; i<attributes.size(); ++i)
  {
    result += attributes[i].size();
  }
  return result;
}

uint32_t BufferLayout::partitionFromUsageType(UsageType ut)
{
  std::map<UsageType, uint32_t>::const_iterator pos = ut2pid.find(ut);
  if(pos == ut2pid.end())
  {
    ostringstream os;
    os << "couldn't find partition for usage type: "<<ut;
    throw runtime_error(os.str());
  }
  return pos->second;
}


}
}