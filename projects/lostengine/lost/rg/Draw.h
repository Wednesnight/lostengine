#ifndef LOST_RG_DRAW_H
#define LOST_RG_DRAW_H

#include "lost/rg/Node.h"
#include "lost/mesh/forward.h"
#include "lost/rg/forward.h"

namespace lost
{
namespace rg
{

/** draws a single mesh.
 * does not process children.
 */
struct Draw : public Node
{
  mesh::MeshPtr mesh;
  
  Draw(mesh::MeshPtr inMesh);
  static DrawPtr create(mesh::MeshPtr inMesh);
  
  void process(gl::ContextPtr ctx);
};
}
}

#endif