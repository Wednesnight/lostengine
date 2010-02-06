#ifndef LOST_GL_HYBRIDVEREXBUFFER_H
#define LOST_GL_HYBRIDVEREXBUFFER_H

#include "lost/gl/HybridBuffer.h"

namespace lost
{
namespace gl
{

struct HybridVertexBuffer;
typedef lost::shared_ptr<HybridVertexBuffer> HybridVertexBufferPtr;

// FIXME: tear out some of this class into a HybridBuffer class that can be a shared base for 
// index/vertex buffer. Theres to much copying happening between these two classes.

// FIXME: partitions need separate dirty flag, not a global one for the whole thing.

struct HybridVertexBuffer : HybridBuffer
{
  HybridVertexBuffer(const BufferLayout& inLayout);
  
};

}
}

#endif