#ifndef LOST_RG_CLEAR_H
#define LOST_RG_CLEAR_H

#include "lost/rg/Node.h"
#include "lost/gl/gltypes.h"

namespace lost
{
namespace rg
{


struct Clear : public Node
{
  GLbitfield mask;
  Clear(GLbitfield inMask);
  static ClearPtr create(GLbitfield inMask);

  void process(gl::ContextPtr& ctx);
};

}
}

#endif