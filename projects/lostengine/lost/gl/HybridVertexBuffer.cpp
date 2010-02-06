#include "lost/gl/HybridVertexBuffer.h"
#include "lost/gl/Context.h"
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
