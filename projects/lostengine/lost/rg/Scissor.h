#ifndef LOST_RG_SCISSOR_H
#define LOST_RG_SCISSOR_H

#include "lost/rg/Node.h"
#include "lost/math/Rect.h"

namespace lost
{
  namespace rg
  {
    
    struct Scissor;
    typedef lost::shared_ptr<Scissor> ScissorPtr;
    /** enables/disables scissoring.
     *  does not process children.
     */
    struct Scissor : public Node
    {
      lost::math::Rect rect;
      Scissor(const math::Rect& inRect);
      static NodePtr create(const math::Rect& inRect);
      
      void process(gl::ContextPtr ctx);
    };
  }
}

#endif
