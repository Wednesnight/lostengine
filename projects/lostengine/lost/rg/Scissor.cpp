#include "lost/rg/Scissor.h"
#include "lost/gl/Context.h"

namespace lost
{
  namespace rg
  {
    
    Scissor::Scissor(bool inEnable)
    : enable(inEnable)
    {
      name = "scissor";
    }
    
    ScissorPtr Scissor::create(bool inEnable)
    {
      return ScissorPtr(new Scissor(inEnable));
    }
    
    void Scissor::process(gl::ContextPtr& ctx)
    {
      if(!active) return;
      ctx->scissor(enable);
    }
    
  }
}
