#ifndef LOST_RG_SCISSORSTACK_H
#define LOST_RG_SCISSORSTACK_H

#include "lost/rg/forward.h"
#include "lost/rg/Node.h"
#include "lost/math/Rect.h"

namespace lost
{
namespace rg
{
struct ScissorStack : public Node
{
  ScissorStack();
  static ScissorStackPtr create();
  void process(gl::ContextPtr& ctx);

  bool        clip;
  math::Rect  rect;
};
}
}

#endif