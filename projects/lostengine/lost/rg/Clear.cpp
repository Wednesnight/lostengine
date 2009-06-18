#include "lost/rg/Clear.h"
#include "lost/gl/Context.h"

namespace lost
{
namespace rg
{

Clear::Clear(GLbitfield inMask)
{
  name = "clear";
  mask = inMask;
}

NodePtr Clear::create(GLbitfield inMask)
{
  return ClearPtr(new Clear(inMask));
}

void Clear::process(gl::ContextPtr ctx)
{
  if(!active) return;
  ctx->clear(mask);
}

}
}