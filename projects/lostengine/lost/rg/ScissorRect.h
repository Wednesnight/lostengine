#ifndef LOST_RG_SCISSORRECT_H
#define LOST_RG_SCISSORRECT_H

#include "lost/rg/Node.h"
#include "lost/math/Rect.h"
#include "lost/rg/forward.h"

namespace lost
{
  namespace rg
  {
    
    /** sets scissoring rect.
     *  does not process children.
     */
    struct ScissorRect : public Node
    {
      lost::math::Rect rect;
      ScissorRect(const math::Rect& inRect);
      static ScissorRectPtr create(const math::Rect& inRect);
      
      void process(gl::ContextPtr ctx);
    };
  }
}

#endif
