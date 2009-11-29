#include "lost/rg/DepthTest.h"
#include "lost/gl/Context.h"

namespace lost
{
namespace rg
{

DepthTest::DepthTest(bool inEnable)
:enable(inEnable)
{
  name = "depthTest";
}

DepthTestPtr DepthTest::create(bool inEnable)
{
  return DepthTestPtr(new DepthTest(inEnable));
}

void DepthTest::process(gl::ContextPtr ctx)
{
  if(!active) return;
  ctx->depthTest(enable);
}

}
}