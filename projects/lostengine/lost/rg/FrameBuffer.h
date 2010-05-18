#ifndef LOST_RG_FRAMEBUFFER_H
#define LOST_RG_FRAMEBUFFER_H

#include "lost/rg/Node.h"
#include "lost/gl/forward.h"

namespace lost
{
namespace rg
{

struct FrameBuffer : public Node
{
  gl::FrameBufferPtr fb;
  FrameBuffer(gl::FrameBufferPtr inFb);
  static FrameBufferPtr create(gl::FrameBufferPtr inFb);
  void process(gl::ContextPtr ctx);
};

}
}

#endif
