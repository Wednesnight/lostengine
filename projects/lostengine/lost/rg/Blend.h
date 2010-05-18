#ifndef LOST_RG_BLEND_H
#define LOST_RG_BLEND_H

#include "lost/rg/Node.h"
#include "lost/rg/forward.h"
namespace lost
{
  namespace rg
  {
    
    /** enables/disables blending.
     *  does not process children.
     */
    struct Blend : public Node
    {
      bool enable;
      Blend(bool inEnable);
      static BlendPtr create(bool inEnable);
      
      void process(gl::ContextPtr ctx);
    };
  }
}

#endif
