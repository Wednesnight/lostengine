#ifndef LOST_GL_HYBRIDBUFFER_H
#define LOST_GL_HYBRIDBUFFER_H

#include "lost/gl/Buffer.h"
#include "lost/gl/HostBuffer.h"

namespace lost
{
namespace gl
{

typedef map<UsageType, string> VertexAttributeMap;

/** base class for hybrid buffers that contain a hostbuffer as well as one or more
 *  gpu buffers.
 *  Only one type of gpu buffer can be managed by a hybrid buffer.
 */
struct HybridBuffer
{
  // we need to separate construction and initialisation because derived classes
  // have to perform more complex setup operations in constructor.
  HybridBuffer();               
  void init(GLenum inGpuBufferType,
            const BufferLayout& inLayout);               
  virtual ~HybridBuffer();
  
  void reset(uint32_t num);

  void set(uint32_t idx, UsageType ut, uint8_t val);
  void set(uint32_t idx, UsageType ut, uint16_t val);
  void set(uint32_t idx, UsageType ut, uint32_t val);
  void set(uint32_t idx, UsageType ut, float val);
  void set(uint32_t idx, UsageType ut, const lost::math::Vec2& val);
  void set(uint32_t idx, UsageType ut, const lost::math::Vec3& val);
  void set(uint32_t idx, UsageType ut, const lost::math::Vec4& val);
  void set(uint32_t idx, UsageType ut, const lost::common::Color& val);  
  
  math::Vec2 getAsVec2(uint32_t idx, UsageType ut);
  math::Vec3 getAsVec3(uint32_t idx, UsageType ut);
  uint32_t getAsU32(uint32_t idx, UsageType ut);
  
  void upload();
  
  bool hasUsageType(UsageType ut);
  Buffer* bufferForUsageType(UsageType ut);  
  const AttributePointerConfig& pointerConfigForUsageType(UsageType ut);
  
  GLenum gpuBufferType; // should be either GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER
  bool dirty;
  HostBufferPtr hostBuffer;
  vector<BufferPtr> gpuBuffers;
  VertexAttributeMap vertexAttributeMap; // maps a usageType to a shader vertex attribute name
};

}
}

#endif