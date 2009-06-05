#ifndef LOST_RG_BLEND_H
#define LOST_RG_BLEND_H

#include "lost/rg/Node.h"

namespace lost
{
  namespace rg
  {
    
    struct Blend;
    typedef lost::shared_ptr<Blend> BlendPtr;
    /** enables/disables blending.
     *  does not process children.
     */
    struct Blend : public Node
    {
      bool enable;
      Blend(bool inEnable);
      static NodePtr create(bool inEnable);
      
      void process(gl::ContextPtr ctx);
    };
  }
}

#endif
