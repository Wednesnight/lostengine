#ifndef LOST_RG_DEPTHTEST_H
#define LOST_RG_DEPTHTEST_H

#include "lost/rg/Node.h"

namespace lost
{
namespace rg
{

/** enables/disables depth testing.
 * does not process children.
 */
struct DepthTest : public Node
{
  bool enable;
  DepthTest(bool inEnable);
  static DepthTestPtr create(bool inEnable);
  
  void process(gl::ContextPtr& ctx);
};
}
}

#endif