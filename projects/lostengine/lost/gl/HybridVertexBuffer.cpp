#include "lost/gl/HybridVertexBuffer.h"

namespace lost
{
namespace gl
{

using namespace math;

HybridVertexBuffer::HybridVertexBuffer(const BufferLayout& inLayout)
{
  init(GL_ARRAY_BUFFER, inLayout);
}


}
}
