#ifndef LOST_GL_HYBRIDVEREXBUFFER_H
#define LOST_GL_HYBRIDVEREXBUFFER_H

#include "lost/gl/HostBuffer.h"
#include "lost/gl/VertexBuffer.h"
#include <vector>

namespace lost
{
namespace gl
{

struct HybridVertexBuffer;
typedef lost::shared_ptr<HybridVertexBuffer> HybridVertexBufferPtr;

// FIXME: tear out some of this class into a HybridBuffer class that can be a shared base for 
// index/vertex buffer. Theres to much copying happening between these two classes.

// FIXME: partitions need separate dirty flag, not a global one for the whole thing.

struct HybridVertexBuffer
{
  HybridVertexBuffer(const BufferLayout& inLayout);
  
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
  
  void upload();
  
  bool hasUsageType(UsageType ut);
  VertexBuffer* bufferForUsageType(UsageType ut);
  
  bool dirty;
  HostBufferPtr hostBuffer;
  std::vector<VertexBufferPtr> vertexBuffers;
};

}
}

#endif