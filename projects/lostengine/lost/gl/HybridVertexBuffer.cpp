#include "lost/gl/HybridVertexBuffer.h"
#include "lost/profiler/Blackbox.h"

namespace lost
{
namespace gl
{

using namespace math;

#define bb_hvb_key "lost.gl.HybridVertexBuffer"

HybridVertexBuffer::HybridVertexBuffer(const BufferLayout& inLayout)
{
  init(GL_ARRAY_BUFFER, inLayout);
  BB_INC(bb_hvb_key);
}

HybridVertexBuffer::~HybridVertexBuffer()
{
  BB_DEC(bb_hvb_key);
}

}
}
