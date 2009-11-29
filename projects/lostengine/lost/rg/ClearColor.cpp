#include "lost/rg/ClearColor.h"
#include "lost/gl/Context.h"

namespace lost
{
namespace rg
{

ClearColor::ClearColor(const common::Color& inCol)
{
  col = inCol;
  name = "clearColor";
}

ClearColorPtr ClearColor::create(const common::Color& inCol)
{
  return ClearColorPtr(new ClearColor(inCol));
}

void ClearColor::process(gl::ContextPtr ctx)
{
  if(!active) return;
  ctx->clearColor(col);
}

}
}