#include "lost/rg/FrameBuffer.h"
#include "lost/gl/Context.h"

namespace lost
{
namespace rg
{

FrameBuffer::FrameBuffer(gl::FrameBufferPtr inFb)
{
  name = "frameBuffer";
  fb = inFb;
}

NodePtr FrameBuffer::create(gl::FrameBufferPtr inFb)
{
  return FrameBufferPtr(new FrameBuffer(inFb));
}

void FrameBuffer::process(gl::ContextPtr ctx)
{
  if(!active) return;
  ctx->frameBuffer(fb);
}

}
}