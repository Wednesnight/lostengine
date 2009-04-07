#ifndef LOST_SG_RENDERER_H
#define LOST_SG_RENDERER_H

#include "lost/sg/Node.h"

namespace lost
{
namespace sg
{
struct Renderer
{
  Renderer();
  virtual ~Renderer();
  
  void render(gl::ContextPtr ctx, NodePtr node);
};
}
}

#endif