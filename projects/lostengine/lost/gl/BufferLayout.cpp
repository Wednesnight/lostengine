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

uint32_t numScalarsForElementType(ElementType et)
{
  uint32_t result = 0;
  switch(et)
  {
    case ET_u8:result=1;break;
    case ET_u16:result=1;break;
    case ET_u32:result=1;break;
    case ET_f32:result=1;break;
    case ET_vec2_u8:result=2;break;
    case ET_vec2_u16:result=2;break;
    case ET_vec2_u32:result=2;break;
    case ET_vec2_f32:result=2;break;
    case ET_vec3_u8:result=3;break;
    case ET_vec3_u16:result=3;break;
    case ET_vec3_u32:result=3;break;
    case ET_vec3_f32:result=3;break;
    case ET_vec4_u8:result=4;break;
    case ET_vec4_u16:result=4;break;
    case ET_vec4_u32:result=4;break;
    case ET_vec4_f32:result=4;break;
    case ET_mat4x4_f32:result=16;break;
  }
  return result;
}

GLenum   glScalarTypeForElementType(ElementType et)
{
  GLenum result = 0;
  switch(et)
  {
    case ET_u8:result=GL_UNSIGNED_BYTE;break;
    case ET_u16:result=GL_UNSIGNED_SHORT;break;
    case ET_u32:result=GL_UNSIGNED_INT;break;
    case ET_f32:result=GL_FLOAT;break;
    case ET_vec2_u8:result=GL_UNSIGNED_BYTE;break;
    case ET_vec2_u16:result=GL_UNSIGNED_SHORT;break;
    case ET_vec2_u32:result=GL_UNSIGNED_INT;break;
    case ET_vec2_f32:result=GL_FLOAT;break;
    case ET_vec3_u8:result=GL_UNSIGNED_BYTE;break;
    case ET_vec3_u16:result=GL_UNSIGNED_SHORT;break;
    case ET_vec3_u32:result=GL_UNSIGNED_INT;break;
    case ET_vec3_f32:result=GL_FLOAT;break;
    case ET_vec4_u8:result=GL_UNSIGNED_BYTE;break;
    case ET_vec4_u16:result=GL_UNSIGNED_SHORT;break;
    case ET_vec4_u32:result=GL_UNSIGNED_INT;break;
    case ET_vec4_f32:result=GL_FLOAT;break;
    case ET_mat4x4_f32:result=GL_FLOAT;break;    
  }
  return result;
}

uint32_t BufferLayout::numScalarsForUsageType(UsageType ut)
{
  ElementType et = ut2et[ut];
  return numScalarsForElementType(et);
}

void BufferLayout::updatePointerConfigs()
{
  ut2apc.clear();
  for(uint32_t i=0; i<attributes.size(); ++i)
  {
    UsageType ut = attributes[i].usageType;
    AttributePointerConfig apc;
    apc.size = numScalarsForUsageType(ut);
    apc.type = glScalarTypeFromUsageType(ut);
    apc.stride = stride(ut);
    apc.offset = (GLvoid*)offset(ut);
    ut2apc[ut] = apc;
  }
}

void BufferLayout::add(ElementType elementType, UsageType usageType, uint32_t p)
{
  attributes.push_back(BufferLayoutAttribute(elementType, usageType, p));
  partitions[p] = true;
  ut2pid[usageType] = p;
  ut2et[usageType] = elementType;
  updatePointerConfigs();
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

bool BufferLayout::hasUsageType(UsageType ut)
{
  std::map<UsageType, uint32_t>::iterator pos = ut2pid.find(ut);
  return (pos != ut2pid.end());
}

GLenum BufferLayout::glScalarTypeFromUsageType(UsageType ut)
{
  return glScalarTypeForElementType(ut2et[ut]);
}

uint32_t BufferLayout::stride(UsageType ut)
{
  return partitionSize(ut2pid[ut]);// - elementSize(ut2et[ut]);
}

uint32_t BufferLayout::offset(UsageType ut)
{
  uint32_t result = 0;
  
  uint32_t pid = ut2pid[ut];
  for(uint32_t i=0; i<attributes.size(); ++i)
  {
    if(attributes[i].usageType == ut) break;
    if(attributes[i].partition==pid)
    {
      result += attributes[i].size();
    }
  }
  
  return result;
}

}
}