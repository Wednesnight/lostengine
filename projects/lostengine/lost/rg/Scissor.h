#ifndef LOST_RG_SCISSOR_H
#define LOST_RG_SCISSOR_H

#include "lost/rg/Node.h"

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
      bool enable;
      Scissor(bool inEnable);
      static ScissorPtr create(bool inEnable);
      
      void process(gl::ContextPtr ctx);
    };
  }
}

#endif
