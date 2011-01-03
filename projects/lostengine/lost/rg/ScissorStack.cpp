#include "lost/rg/ScissorStack.h"
#include "lost/gl/Context.h"

namespace lost
{
namespace rg
{

ScissorStack::ScissorStack()
{
  clip = false;
}

ScissorStackPtr ScissorStack::create()
{
  return ScissorStackPtr(new ScissorStack);
}

void ScissorStack::process(gl::ContextPtr& ctx)
{
  if(clip) 
  {
    ctx->pushClippedScissorRect(rect);
  }
  Node::process(ctx);
  if(clip)
  {
    ctx->popScissorRect();
  }
}

}
}