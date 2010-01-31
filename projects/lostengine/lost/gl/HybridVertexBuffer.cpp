#include "lost/gl/HybridVertexBuffer.h"

namespace lost
{
namespace gl
{

HybridVertexBuffer::HybridVertexBuffer(const BufferLayout& inLayout)
{
  hostBuffer.reset(new HostBuffer(inLayout));

  for(uint32_t i=0; i<hostBuffer->layout.partitions.size(); ++i)
  {
    vertexBuffers.push_back(VertexBufferPtr(new VertexBuffer));
  }
  dirty = false;
}

void HybridVertexBuffer::reset(uint32_t num)
{
  hostBuffer->reset(num);
  dirty = true;
}

void HybridVertexBuffer::set(uint32_t idx, UsageType ut, uint8_t val)
{
  hostBuffer->set(idx, ut, val);
  dirty = true;
}

void HybridVertexBuffer::set(uint32_t idx, UsageType ut, uint16_t val)
{
  hostBuffer->set(idx, ut, val);
  dirty = true;
}

void HybridVertexBuffer::set(uint32_t idx, UsageType ut, uint32_t val)
{
  hostBuffer->set(idx, ut, val);
  dirty = true;
}

void HybridVertexBuffer::set(uint32_t idx, UsageType ut, float val)
{
  hostBuffer->set(idx, ut, val);
  dirty = true;
}

void HybridVertexBuffer::set(uint32_t idx, UsageType ut, const lost::math::Vec2& val)
{
  hostBuffer->set(idx, ut, val);
  dirty = true;
}

void HybridVertexBuffer::set(uint32_t idx, UsageType ut, const lost::math::Vec3& val)
{
  hostBuffer->set(idx, ut, val);
  dirty = true;
}

void HybridVertexBuffer::set(uint32_t idx, UsageType ut, const lost::math::Vec4& val)
{
  hostBuffer->set(idx, ut, val);
  dirty = true;
}

void HybridVertexBuffer::set(uint32_t idx, UsageType ut, const lost::common::Color& val)
{
  hostBuffer->set(idx, ut, val);
  dirty = true;
}

void HybridVertexBuffer::upload()
{
  // FIXME: upload partition data to corresponding vertex buffers
  dirty = false;
}

}
}
