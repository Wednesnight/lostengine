#ifndef LOST_GL_HYBRIDVEREXBUFFER_H
#define LOST_GL_HYBRIDVEREXBUFFER_H

#include "lost/gl/forward.h"
#include "lost/gl/HybridBuffer.h"


namespace lost
{
namespace gl
{

// FIXME: partitions need separate dirty flag, not a global one for the whole thing.

struct HybridVertexBuffer : HybridBuffer
{
  HybridVertexBuffer(const BufferLayout& inLayout);
  ~HybridVertexBuffer();
  
};

}
}

#endif