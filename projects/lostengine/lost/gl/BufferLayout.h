#ifndef LOST_GL_BUFFERLAYOUT_H
#define LOST_GL_BUFFERLAYOUT_H

#include <vector>
#include <map>

namespace lost
{
namespace gl
{

// these are the basic elements that can be used to define a buffer layout
enum ElementType
{
  ET_u8 = 0,
  ET_u16 = 1,
  ET_u32 = 2,
  ET_f32 = 3,
  
  ET_vec2_u8 = 10,
  ET_vec2_u16 = 11,
  ET_vec2_u32 = 12,
  ET_vec2_f32 = 13,
  
  ET_vec3_u8 = 20,
  ET_vec3_u16 = 21,
  ET_vec3_u32 = 22,
  ET_vec3_f32 = 23,
  
  ET_vec4_u8 = 30,
  ET_vec4_u16 = 31,
  ET_vec4_u32 = 32,
  ET_vec4_f32 = 33,
  
  ET_mat4x4_f32 = 40
};

uint32_t elementSize(ElementType t);

// each element or stride of a buffer needs to be marked with a usage type, so it can be mapped to a GL buffer and stride
enum UsageType
{
  UT_unused = 0,  // elements marked with this usage type will be ignored, handy for testing or padding to multiples of 32/64 bytes for performance optimisation
  UT_index,       // special case for IndexBuffers, the element is an index into a group of vertices, used for drawing
  UT_vertex,      // element is a vertex coordinate, usually a vec2/3/4 variant
  UT_texcoord0,   // element is a texture coordinate and should be mapped to texture unit 0
  UT_texcoord1,   // element is a texture coordinate and should be mapped to texture unit 1
  UT_texcoord2,   // element is a texture coordinate and should be mapped to texture unit 2
  UT_texcoord3,   // element is a texture coordinate and should be mapped to texture unit 3
  UT_normal,      // element is a normal vector
  UT_color,       // element is a color, usually a vec3/vec4 variant
  UT_vertexAttrib0, // element should be mapped to vertex attribute 0 of a shader program
  UT_vertexAttrib1, // element should be mapped to vertex attribute 1 of a shader program
  UT_vertexAttrib2, // element should be mapped to vertex attribute 2 of a shader program
  UT_vertexAttrib3, // element should be mapped to vertex attribute 3 of a shader program
  UT_vertexAttrib4, // element should be mapped to vertex attribute 4 of a shader program
  UT_vertexAttrib5, // element should be mapped to vertex attribute 5 of a shader program
  UT_vertexAttrib6, // element should be mapped to vertex attribute 6 of a shader program
  UT_vertexAttrib7  // element should be mapped to vertex attribute 7 of a shader program
};

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
  
  void add(ElementType et, UsageType ut, uint32_t p);

  // number of partitions in this layout
  uint32_t numPartitions() const;

  // size of a particular partition in bytes
  uint32_t partitionSize(uint32_t partitionId) const;
   
  // size of the whole layout in bytes, without taking partitioning into account, in bytes
  uint32_t size() const;
  
  uint32_t partitionFromUsageType(UsageType ut);
};


} // gl
} // lost

#endif
