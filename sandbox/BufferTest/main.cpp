#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <stdexcept>

using namespace std;

// test types for setters/getters
struct Vec2
{
  float x;
  float y;
};

struct Vec3
{
  float x;
  float y;
  float z;
};

struct Vec4
{
  float x;
  float y;
  float z;
  float w;  
};

struct Color
{
  float r;
  float g;
  float b;
  float a;  
};

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

uint32_t elementSize(ElementType t)
{
  uint32_t result = 0;
  switch (t) {
    case ET_i8:result=1;break;
    case ET_i16:result=2;break;
    case ET_i32:result=4;break;
    case ET_f32:result=4;break;
    case ET_vec2_i8:result=2;break;
    case ET_vec2_i16:result=4;break;
    case ET_vec2_i32:result=8;break;
    case ET_vec2_f32:result=8;break;
    case ET_vec3_i8:result=3;break;
    case ET_vec3_i16:result=6;break;
    case ET_vec3_i32:result=12;break;
    case ET_vec3_f32:result=12;break;
    case ET_vec4_i8:result=4;break;
    case ET_vec4_i16:result=8;break;
    case ET_vec4_i32:result=16;break;
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
struct HostBufferAttribute
{
  ElementType elementType;
  UsageType   usageType;
  uint32_t    partition;
  
  HostBufferAttribute(ElementType et, UsageType ut, uint32_t p) : elementType(et), usageType(ut), partition(p) {}
  
  uint32_t size() const { return elementSize(elementType);}
};

struct HostBufferLayout
{
  // FIXME: add caching for various size and partition parameters so we don'T have to recalculate on every set call
  // add maps that hold these values. Update maps upon each add call.
  std::vector<HostBufferAttribute> attributes;
  std::map<uint32_t, bool> partitions;
  std::map<UsageType, uint32_t> ut2pid; // usage type to partition id
  
  void add(ElementType et, UsageType ut, uint32_t p)
  {
    attributes.push_back(HostBufferAttribute(et, ut, p));
    partitions[p] = true;
    ut2pid[ut] = p;
  }

  // number of partitions in this layout
  uint32_t numPartitions() const
  {
    return partitions.size();
  }

  // size of a particular partition in bytes
  uint32_t partitionSize(uint32_t partitionId) const
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
  uint32_t size() const
  {
    uint32_t result = 0;
    for(uint32_t i=0; i<attributes.size(); ++i)
    {
      result += attributes[i].size();
    }
    return result;
  }
  
  uint32_t partitionFromUsageType(UsageType ut)
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
};

struct HostBuffer
{
  HostBufferLayout  layout;
  std::vector<unsigned char*> partitions; // the actual physical buffers
  
  void init(const HostBufferLayout& inLayout)
  {
    layout = inLayout;
    cout << "layout size: "<<layout.size() << endl;
    uint32_t nump = layout.numPartitions();
    cout << "partitions: "<< nump << endl;
    for(uint32_t i=0; i<nump; ++i)
    {
      cout << "partition " << i << " size " << layout.partitionSize(i) << endl;
    }    
  }
  
  HostBuffer(const HostBufferLayout& inLayout)
  {
    init(inLayout);
  }

  HostBuffer(const HostBufferLayout& inLayout, uint32_t num)
  {
    init(inLayout);
    reset(num);
  }
  
  void deleteAllPartitions()
  {
    for(uint32_t i=0; i<partitions.size(); ++i)
    {
      cout << "deleting partition " << i << endl;
      free(partitions[i]);
    }
    partitions.clear();
  }  

  // resizes the buffer to accomodate num structs with the current layout. if num is 0, the buffer will be cleared
  void reset(uint32_t num)
  {
    if(num == 0)
    {
      deleteAllPartitions();
    }
    else
    {
      deleteAllPartitions();
      for(uint32_t i=0; i<layout.numPartitions(); ++i)
      {
        uint32_t s = layout.partitionSize(i)*num;
        partitions.push_back((unsigned char*)malloc(s));
        cout << "allocated p:"<<i<<" num:"<<num<<" bytes:"<<s<<endl;
      }
    }
  }
    
  void set(uint32_t idx, UsageType ut, float val)
  {
    uint32_t pid = layout.partitionFromUsageType(ut);
    unsigned char* p = partitions[pid];
    uint32_t num = sizeof(val);
    p = p+idx*num;
    float* vp = (float*)p;
    memcpy(vp, &val, num);
  }

  void set(uint32_t idx, UsageType ut, const Vec2& val)
  {
    uint32_t pid = layout.partitionFromUsageType(ut);
    unsigned char* p = partitions[pid];
    uint32_t num = sizeof(val);
    p = p+idx*num;
    Vec2* vp = (Vec2*)p;
    memcpy(vp, &val, num);
  }

  void set(uint32_t idx, UsageType ut, const Vec3& val)
  {
    uint32_t pid = layout.partitionFromUsageType(ut);
    unsigned char* p = partitions[pid];
    uint32_t num = sizeof(val);
    p = p+idx*num;
    Vec3* vp = (Vec3*)p;
    memcpy(vp, &val, num);
  }

  void set(uint32_t idx, UsageType ut, const Vec4& val)
  {
    uint32_t pid = layout.partitionFromUsageType(ut);
    unsigned char* p = partitions[pid];
    uint32_t num = sizeof(val);
    p = p+idx*num;
    Vec4* vp = (Vec4*)p;
    memcpy(vp, &val, num);
  }

  void set(uint32_t idx, UsageType ut, const Color& val)
  {
    uint32_t pid = layout.partitionFromUsageType(ut);
    unsigned char* p = partitions[pid];
    uint32_t num = sizeof(val);
    p = p+idx*num;
    Color* vp = (Color*)p;
    memcpy(vp, &val, num);
  }

};

int main (int argc, char * const argv[]) {
    HostBufferLayout layout;
    layout.add(ET_vec4_f32, UT_vertex, 0);
    layout.add(ET_vec3_f32, UT_normal, 0);
    layout.add(ET_vec2_f32, UT_texcoord0, 0);

    layout.add(ET_f32, UT_vertexAttrib0, 1);
    layout.add(ET_f32, UT_vertexAttrib1, 1);

    HostBuffer buffer1(layout);
    HostBuffer buffer2(layout, 100);
    HostBuffer buffer3(layout);

    buffer3.reset(50);
    buffer3.reset(0);
    
    Vec4 v4;
    buffer2.set(0, UT_vertex, v4);
    buffer2.set(1, UT_vertex, v4);
    buffer2.set(2, UT_vertex, v4);
    
    return 0;
}
