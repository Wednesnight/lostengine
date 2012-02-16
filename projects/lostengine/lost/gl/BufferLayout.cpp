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

const char* nameForElementType(ElementType et)
{
  const char* result = "";

#define ETNAME(ett)  case ett: result = #ett;break;
  
  switch(et)
  {
    ETNAME(ET_u8);
    ETNAME(ET_u16);
    ETNAME(ET_u32);
    ETNAME(ET_f32);
    ETNAME(ET_vec2_u8);
    ETNAME(ET_vec2_u16);
    ETNAME(ET_vec2_u32);
    ETNAME(ET_vec2_f32);
    ETNAME(ET_vec3_u8);
    ETNAME(ET_vec3_u16);
    ETNAME(ET_vec3_u32);
    ETNAME(ET_vec3_f32);
    ETNAME(ET_vec4_u8);
    ETNAME(ET_vec4_u16);
    ETNAME(ET_vec4_u32);
    ETNAME(ET_vec4_f32);
    ETNAME(ET_mat4x4_f32);
    default:result="???";
  }
  
  return result;
}
  
const char* nameForUsageType(UsageType ut)
{
  const char* result = "";
  
#define UTNAME(utt)  case utt: result = #utt;break;
  
  switch(ut)
  {
    UTNAME(UT_unused);
    UTNAME(UT_index);
    UTNAME(UT_position);
    UTNAME(UT_normal);
    UTNAME(UT_color);
    UTNAME(UT_texcoord0);
    UTNAME(UT_texcoord1);  
    UTNAME(UT_texcoord2);  
    UTNAME(UT_texcoord3);  
    UTNAME(UT_texcoord4);
    UTNAME(UT_texcoord5);
    UTNAME(UT_texcoord6);
    UTNAME(UT_texcoord7);
    UTNAME(UT_texcoord8);
    UTNAME(UT_texcoord9);
    UTNAME(UT_texcoord10);
    UTNAME(UT_texcoord11);
    UTNAME(UT_texcoord12);
    UTNAME(UT_texcoord13);
    UTNAME(UT_texcoord14);
    UTNAME(UT_texcoord15);
    UTNAME(UT_vertexAttrib0);
    UTNAME(UT_vertexAttrib1);
    UTNAME(UT_vertexAttrib2);
    UTNAME(UT_vertexAttrib3);
    UTNAME(UT_vertexAttrib4);
    UTNAME(UT_vertexAttrib5);
    UTNAME(UT_vertexAttrib6);
    UTNAME(UT_vertexAttrib7);
    UTNAME(UT_vertexAttrib8);
    UTNAME(UT_vertexAttrib9);
    UTNAME(UT_vertexAttrib10);
    UTNAME(UT_vertexAttrib11);
    UTNAME(UT_vertexAttrib12);
    UTNAME(UT_vertexAttrib13);
    UTNAME(UT_vertexAttrib14);
    UTNAME(UT_vertexAttrib15);
  }
  return result;
}
  
  
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
