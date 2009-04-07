#include "lost/sg/Renderer.h"

namespace lost
{
namespace sg
{

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}
  
void Renderer::render(gl::ContextPtr ctx, NodePtr node)
{
  DOUT("rendering node: "<<node->name);
  // check if the current node has child nodes and process them first
  if(node->nodes.size() > 0)
  {
    for(std::list<NodePtr>::iterator i=node->nodes.begin();
        i!=node->nodes.end();
        ++i)
    {
      render(ctx, *i);
    }
  }
  
  // apply framebuffer
  if(node->framebuffer)
  {
    DOUT("applying "<<node->name<<" framebuffer");
    node->framebuffer->enable();
  }
  
  // apply camera
  if(node->camera)
  {
    node->camera->apply();
  }
  
  // apply additional state
  if(node->state)
  {
    ctx->apply(node->state);
  }
  
  // apply material
  if(node->material)
  {
    node->material->apply(ctx);
  }
  
  // transform
//  if()
//  glMatrixMode(GL_MODELVIEW);

  // meshes
  if(node->meshes.size() > 0)
  {
    for(std::list<MeshPtr>::iterator i = node->meshes.begin();
        i != node->meshes.end();
        ++i)
    {
      (*i)->render(ctx);
    }
  }
  
}

}
}

