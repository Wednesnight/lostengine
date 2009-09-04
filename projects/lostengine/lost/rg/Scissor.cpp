#include "lost/rg/Scissor.h"
#include "lost/gl/Context.h"

namespace lost
{
  namespace rg
  {
    
    Scissor::Scissor(const math::Rect& inRect)
    : rect(inRect)
    {
      name = "scissor";
    }
    
    NodePtr Scissor::create(const math::Rect& inRect)
    {
      return ScissorPtr(new Scissor(inRect));
    }
    
    void Scissor::process(gl::ContextPtr ctx)
    {
      if(!active) return;
      if (!rect)
      {
        ctx->scissor(false);
      }
      else
      {
        ctx->scissorRect(rect);
        ctx->scissor(true);
      }
    }
    
  }
}