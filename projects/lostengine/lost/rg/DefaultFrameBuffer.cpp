#include "lost/rg/DefaultFrameBuffer.h"
#include "lost/gl/Context.h"

namespace lost
{
namespace rg
{

NodePtr DefaultFrameBuffer::create()
{
  return DefaultFrameBufferPtr(new DefaultFrameBuffer);
}

void DefaultFrameBuffer::process(gl::ContextPtr ctx)
{
  if(!active) return;
  ctx->defaultFrameBuffer();
}

}
}