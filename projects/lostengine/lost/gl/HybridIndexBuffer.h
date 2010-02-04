#ifndef LOST_GL_HYBRIDINDEXBUFFER_H
#define LOST_GL_HYBRIDINDEXBUFFER_H

#include "lost/gl/HostBuffer.h"
#include "lost/gl/IndexBuffer.h"

namespace lost
{
namespace gl
{

struct HybridIndexBuffer;
typedef lost::shared_ptr<HybridIndexBuffer> HybridIndexBufferPtr;

struct HybridIndexBuffer
{
  // since an IndexBuffer is only an array of integers, we can simplify the 
  // layouting of the buffer by only requiring a single element type parameter.
  HybridIndexBuffer(ElementType et);
  
  void reset(uint32_t num);

  void set(uint32_t idx, UsageType ut, uint8_t val);
  void set(uint32_t idx, UsageType ut, uint16_t val);
  void set(uint32_t idx, UsageType ut, uint32_t val);
  
  void upload();

  bool hasUsageType(UsageType ut);
  
  bool dirty;
  HostBufferPtr hostBuffer;
  IndexBufferPtr indexBuffer;
  GLenum type;
};

}
}

#endif