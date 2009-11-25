#include "lost/rg/Draw.h"
#include "lost/mesh/Mesh.h"
#include "lost/gl/Context.h"

namespace lost
{
namespace rg
{

Draw::Draw(mesh::MeshPtr inMesh)
: mesh(inMesh)
{
  name = "draw";
}

NodePtr Draw::create(mesh::MeshPtr inMesh)
{
  return DrawPtr(new Draw(inMesh));
}

void Draw::process(gl::ContextPtr ctx)
{
  if(!active) return;
  ctx->draw(mesh);
}

}
}