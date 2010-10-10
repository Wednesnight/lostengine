#include "lost/rg/Blend.h"
#include "lost/gl/Context.h"

namespace lost
{
  namespace rg
  {
    
    Blend::Blend(bool inEnable)
    :enable(inEnable)
    {
      name = "blend";
    }
    
    BlendPtr Blend::create(bool inEnable)
    {
      return BlendPtr(new Blend(inEnable));
    }
    
    void Blend::process(gl::ContextPtr& ctx)
    {
      if(!active) return;
      ctx->blend(enable);
    }
    
  }
}