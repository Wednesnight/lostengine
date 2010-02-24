#ifndef LOST_GL_BUFFERLAYOUT_H
#define LOST_GL_BUFFERLAYOUT_H

#include <vector>
#include <map>
#include "lost/gl/gltypes.h"

namespace lost
{
namespace gl
{


// these are the basic elements that can be used to define a buffer layout
typedef uint32_t ElementType;
const static ElementType ET_u8 = 0;
const static ElementType ET_u16 = 1;
const static ElementType ET_u32 = 2;
const static ElementType ET_f32 = 3;

const static ElementType ET_vec2_u8 = 10;
const static ElementType ET_vec2_u16 = 11;
const static ElementType ET_vec2_u32 = 12;
const static ElementType ET_vec2_f32 = 13;

const static ElementType ET_vec3_u8 = 20;
const static ElementType ET_vec3_u16 = 21;
const static ElementType ET_vec3_u32 = 22;
const static ElementType ET_vec3_f32 = 23;

const static ElementType ET_vec4_u8 = 30;
const static ElementType ET_vec4_u16 = 31;
const static ElementType ET_vec4_u32 = 32;
const static ElementType ET_vec4_f32 = 33;

const static ElementType ET_mat4x4_f32 = 40;


uint32_t elementSize(ElementType t); // returns elemtn size in bytes
uint32_t numScalarsForElementType(ElementType et); // returns the number of scalars an ET is made of. 
                                                   // This is mostly relevant for vectpr/matrix since all other types return 1.
GLenum   glScalarTypeForElementType(ElementType et);
// each element or stride of a buffer needs to be marked with a usage type, so it can be mapped to a GL buffer and stride
typedef uint32_t UsageType;
const static UsageType UT_unused = 0;  // elements marked with this usage type will be ignored, handy for testing or padding to multiples of 32/64 bytes for performance optimisation
const static UsageType UT_index = 1;       // special case for IndexBuffers, the element is an index into a group of vertices, used for drawing
const static UsageType UT_position = 2;      // element is a vertex coordinate, usually a vec2/3/4 variant
const static UsageType UT_texcoord0 = 3;   // element is a texture coordinate and should be mapped to texture unit 0
const static UsageType UT_texcoord1 = 4;   // element is a texture coordinate and should be mapped to texture unit 1
const static UsageType UT_texcoord2 = 5;   // element is a texture coordinate and should be mapped to texture unit 2
const static UsageType UT_texcoord3 = 6;   // element is a texture coordinate and should be mapped to texture unit 3
const static UsageType UT_normal = 7;      // element is a normal vector
const static UsageType UT_color = 8;       // element is a color, usually a vec3/vec4 variant
const static UsageType UT_vertexAttrib0 = 9; // element should be mapped to vertex attribute 0 of a shader program
const static UsageType UT_vertexAttrib1 = 10; // element should be mapped to vertex attribute 1 of a shader program
const static UsageType UT_vertexAttrib2 = 11; // element should be mapped to vertex attribute 2 of a shader program
const static UsageType UT_vertexAttrib3 = 12; // element should be mapped to vertex attribute 3 of a shader program
const static UsageType UT_vertexAttrib4 = 13; // element should be mapped to vertex attribute 4 of a shader program
const static UsageType UT_vertexAttrib5 = 14; // element should be mapped to vertex attribute 5 of a shader program
const static UsageType UT_vertexAttrib6 = 15; // element should be mapped to vertex attribute 6 of a shader program
const static UsageType UT_vertexAttrib7 = 16; // element should be mapped to vertex attribute 7 of a shader program

// elements must be paired with usage types
struct BufferLayoutAttribute
{
  ElementType elementType;
  UsageType   usageType;
  uint32_t    partition;
  
  BufferLayoutAttribute(ElementType et, UsageType ut, uint32_t p) : elementType(et), usageType(ut), partition(p) {}
  
  uint32_t size() const { return elementSize(elementType);}
};

struct BufferLayout
{
  // FIXME: add caching for various size and partition parameters so we don'T have to recalculate on every set call
  // add maps that hold these values. Update maps upon each add call.
  std::vector<BufferLayoutAttribute> attributes;
  std::map<uint32_t, bool> partitions;
  std::map<UsageType, uint32_t> ut2pid; // usage type to partition id
  std::map<UsageType, ElementType> ut2et;
  
  void add(ElementType elementType, UsageType usageType, uint32_t p);

  // number of partitions in this layout
  uint32_t numPartitions() const;

  // size of a particular partition in bytes
  uint32_t partitionSize(uint32_t partitionId) const;
   
  // size of the whole layout in bytes, without taking partitioning into account, in bytes
  uint32_t size() const;
  
  uint32_t partitionFromUsageType(UsageType ut);
  
  bool hasUsageType(UsageType ut);
  GLenum glScalarTypeFromUsageType(UsageType ut);
  uint32_t stride(UsageType ut);
  uint32_t offset(UsageType ut);
};


} // gl
} // lost

#endif
