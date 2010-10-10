#ifndef LOST_RG_SCISSOR_H
#define LOST_RG_SCISSOR_H

#include "lost/rg/Node.h"
#include "lost/rg/forward.h"

namespace lost
{
  namespace rg
  {
    
    /** enables/disables scissoring.
     *  does not process children.
     */
    struct Scissor : public Node
    {
      bool enable;
      Scissor(bool inEnable);
      static ScissorPtr create(bool inEnable);
      
      void process(gl::ContextPtr& ctx);
    };
  }
}

#endif
