#include <iostream>
#include <vector>

using namespace std;

/** Framework for runtime creation of interleaved buffers.
 * These buffers are intended to be used with GL for efficient memory management and also efficient buffer usage.
 * Therefore, all information must be avaiilable at runtime, so it can be stuffed into the appropriate GL calls, but one of the 
 * main goals is to deduce as much information as possible from the basic structure and usage types.
 * 
 */

// these are the basic elements that can be used to define a buffer layout
enum ElementType
{
  ET_i8 = 0,
  ET_i16 = 1,
  ET_i32 = 2,
  ET_f32 = 3,
  
  ET_vec2_i8 = 10,
  ET_vec2_i16 = 11,
  ET_vec2_i32 = 12,
  ET_vec2_f32 = 13,
  
  ET_vec3_i8 = 20,
  ET_vec3_i16 = 21,
  ET_vec3_i32 = 22,
  ET_vec3_f32 = 23,
  
  ET_vec4_i8 = 30,
  ET_vec4_i16 = 31,
  ET_vec4_i32 = 32,
  ET_vec4_f32 = 33,
  
  ET_mat4x4_f32 = 40
};

// these are the sizes of the elements in bytes
enum ElementSize
{
  ES_i8 = 1,
  ES_i16 = 2,
  ES_i32 = 4,
  ES_f32 = 4,
  
  ES_vec2_i8 = 2,
  ES_vec2_i16 = 4,
  ES_vec2_i32 = 8,
  ES_vec2_f32 = 8,
  
  ES_vec3_i8 = 3,
  ES_vec3_i16 = 6,
  ES_vec3_i32 = 12,
  ES_vec3_f32 = 12,
  
  ES_vec4_i8 = 4,
  ES_vec4_i16 = 8,
  ES_vec4_i32 = 16,
  ES_vec4_f32 = 16,

  ES_mat4x4_f32 = 64
};

// buffer types denote the intended usage of a buffer object when it is sent to the graphics hardware
enum BufferType
{
  BT_vertex = 0, // the contained data is to be interpreted 
  BT_index = 1
};

// each element or stride of a buffer needs to be marked with a usage type, so it can be mapped to a GL buffer and stride
enum UsageType
{
  UT_unused = 0,  // elements marked with this usage type will be ignored, might be handy for testing 
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
struct ElementMapping
{
  ElementType elementType;
  UsageType   usageType;
  
  ElementMapping(ElementType et, UsageType ut) : elementType(et), usageType(ut) {}
};

// in order to create a buffer, you must first create a BufferLayout which contains a number of elements that make up a struct. A buffer
// ten contains an array of these structs. A StructLayout can contain any amount and type of elements, but always at least one. Just 
// like with C structs, the order is relevant. 
typedef vector<ElementMapping> StructLayout;


struct Buffer
{
  StructLayout  layout;
  BufferType    bufferType;
  
  Buffer(const StructLayout& inLayout, BufferType inType) : layout(inLayout), bufferType(inType) {}
};

int main (int argc, char * const argv[]) {

    StructLayout layout;
    layout.push_back(ElementMapping(ET_vec3_f32, UT_vertex));
    layout.push_back(ElementMapping(ET_vec3_f32, UT_normal));
    layout.push_back(ElementMapping(ET_vec2_f32, UT_texcoord0));

    Buffer buffer(layout, BT_vertex);
  
    return 0;
}
