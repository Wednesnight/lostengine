#include "lost/rg/ScissorRect.h"
#include "lost/gl/Context.h"

namespace lost
{
  namespace rg
  {
    
    ScissorRect::ScissorRect(const math::Rect& inRect)
    : rect(inRect)
    {
      name = "scissorRect";
    }
    
    NodePtr ScissorRect::create(const math::Rect& inRect)
    {
      return ScissorRectPtr(new ScissorRect(inRect));
    }
    
    void ScissorRect::process(gl::ContextPtr ctx)
    {
      if(!active) return;
      ctx->scissorRect(rect);
    }
    
  }
}