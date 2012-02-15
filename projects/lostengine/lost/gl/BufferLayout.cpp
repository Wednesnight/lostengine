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

#include "lost/gl/BufferLayout.h"
#include <sstream>
#include <stdexcept>

namespace lost
{
namespace gl
{

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
      std::ostringstream os;
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

// size of a single struct in bytes
/*uint32_t BufferLayout::structSize() const
{
  uint32_t result = 0;
  
  for(uint32_t i=0; i<attributes.size(); ++i)
  {
    result += attributes[i].size();
  }
  
  return result;
}*/
  
  
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
    apc.normalise = attributes[i].normalise;
    ut2apc[ut] = apc;
  }
}

void BufferLayout::add(ElementType elementType, UsageType usageType)
{
  attributes.push_back(BufferLayoutAttribute(elementType, usageType));
  ut2et[usageType] = elementType;
  updatePointerConfigs();
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

bool BufferLayout::hasUsageType(UsageType ut)
{
  map<UsageType, ElementType>::iterator pos = ut2et.find(ut);
  return (pos != ut2et.end());
}

GLenum BufferLayout::glScalarTypeFromUsageType(UsageType ut)
{
  return glScalarTypeForElementType(ut2et[ut]);
}

uint32_t BufferLayout::stride(UsageType ut)
{
  return size();
}

uint32_t BufferLayout::offset(UsageType ut)
{
  uint32_t result = 0;
  
  for(uint32_t i=0; i<attributes.size(); ++i)
  {
    if(attributes[i].usageType == ut) break;
    result += attributes[i].size();
  }
  
  return result;
}

}
}
