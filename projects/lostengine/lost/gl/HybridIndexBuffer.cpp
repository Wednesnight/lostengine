#include "lost/gl/HybridIndexBuffer.h"

namespace lost
{
namespace gl
{

HybridIndexBuffer::HybridIndexBuffer(ElementType et)
{
  // an indexbuffer only ever has one attribute with usage type index in a single partition
  // only the element type can vary, to optimize the buffer for hardware requirements or
  // mesh sizes.
  BufferLayout layout;
  layout.add(et, UT_index, 0);
  dirty = false;
  hostBuffer.reset(new HostBuffer(layout));
  indexBuffer.reset(new IndexBuffer);
}

void HybridIndexBuffer::reset(uint32_t num)
{
  hostBuffer->reset(num);
  dirty = true;
}

void HybridIndexBuffer::set(uint32_t idx, UsageType ut, uint8_t val)
{
  hostBuffer->set(idx, ut, val);
  dirty = true;
}

void HybridIndexBuffer::set(uint32_t idx, UsageType ut, uint16_t val)
{
  hostBuffer->set(idx, ut, val);
  dirty = true;
}

void HybridIndexBuffer::set(uint32_t idx, UsageType ut, uint32_t val)
{
  hostBuffer->set(idx, ut, val);
  dirty = true;
}

void HybridIndexBuffer::upload()
{
  indexBuffer->bind();
  indexBuffer->bufferData(indexBuffer->target, 
                          hostBuffer->count*hostBuffer->layout.partitionSize(0),// indexbuffer only ever has one partition
                          hostBuffer->partitions[0],
                          GL_STATIC_DRAW); // FIXME: this parameter should probably be configurable
  dirty = false;
}

}
}
