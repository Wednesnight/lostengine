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
  
  bool dirty;
  HostBufferPtr hostBuffer;
  std::vector<VertexBufferPtr> vertexBuffers;
};

}
}

#endif