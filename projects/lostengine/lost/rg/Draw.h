#ifndef LOST_RG_DRAW_H
#define LOST_RG_DRAW_H

#include "lost/rg/Node.h"

namespace lost
{
namespace mesh
{
struct Mesh;
typedef lost::shared_ptr<Mesh> MeshPtr;
}
namespace rg
{
struct Draw;
typedef lost::shared_ptr<Draw> DrawPtr;

/** draws a single mesh.
 * does not process children.
 */
struct Draw : public Node
{
  mesh::MeshPtr mesh;
  
  Draw(mesh::MeshPtr inMesh);
  static NodePtr create(mesh::MeshPtr inMesh);
  
  void process(gl::ContextPtr ctx);
};
}
}

#endif