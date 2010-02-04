#include "lost/gl/HybridVertexBuffer.h"
#include "lost/gl/Context.h"
namespace lost
{
namespace gl
{

using namespace math;

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

Vec2 HybridVertexBuffer::getAsVec2(uint32_t idx, UsageType ut)
{
  return hostBuffer->getAsVec2(idx, ut);
}

void HybridVertexBuffer::upload()
{
  for(uint32_t i=0; i<hostBuffer->partitions.size(); ++i)
  {
    VertexBuffer* vb = vertexBuffers[i].get();
    Context::getCurrent()->bind(vb);
//    vb->bind();
    vb->bufferData(vb->target, 
                  hostBuffer->count*hostBuffer->layout.partitionSize(i), 
                  hostBuffer->partitions[i],
                  GL_STATIC_DRAW); // FIXME: this parameter should probably be configurable
  }
  dirty = false;
}

bool HybridVertexBuffer::hasUsageType(UsageType ut)
{
  return hostBuffer->hasUsageType(ut);
}

VertexBuffer* HybridVertexBuffer::bufferForUsageType(UsageType ut)
{
  uint32_t pid = hostBuffer->layout.ut2pid[ut];
  return vertexBuffers[pid].get();
}

}
}
