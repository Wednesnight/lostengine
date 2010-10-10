#ifndef LOST_RG_DEFAULTFRAMEBUFFER_H
#define LOST_RG_DEFAULTFRAMEBUFFER_H

#include "lost/rg/Node.h"

namespace lost
{
namespace rg
{

struct DefaultFrameBuffer : public Node
{
  static DefaultFrameBufferPtr create();
  void process(gl::ContextPtr& ctx);
};

}
}

#endif
