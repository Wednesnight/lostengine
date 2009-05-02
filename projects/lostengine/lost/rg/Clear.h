#ifndef LOST_RG_CLEAR_H
#define LOST_RG_CLEAR_H

#include "lost/rg/Node.h"

namespace lost
{
namespace rg
{

struct Clear;
typedef lost::shared_ptr<Clear> ClearPtr;

struct Clear : public Node
{
  GLbitfield mask;
  Clear(GLbitfield inMask);
  static NodePtr create(GLbitfield inMask);

  void process(gl::ContextPtr ctx);
};

}
}

#endif