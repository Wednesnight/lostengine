#ifndef LOST_SG_NODE
#define LOST_SG_NODE

#include "lost/camera/Camera.h"
#include "lost/gl/FrameBuffer.h"
#include "lost/sg/Material.h"
#include "lost/sg/Mesh.h"
#include <list>
#include <string>

namespace lost
{
namespace sg
{

struct Node;
typedef boost::shared_ptr<Node> NodePtr;
  
/** A container for state information, geometry, transforms and materials.
 * The render order:
 * * Nodes are rendered first, recursively
 * * if no node is there to render
 *  * apply framebuffer (optional)
 *  * apply camera (optional)
 *  * apply additional state (optional)
 *  * apply material (optional)
 *  * apply transform (modelview, set globally, NOT multiplied with parent state)
 *  * render meshes (optional)
 */
struct Node 
{
  Node();
  virtual ~Node();

  std::string             name;

  std::list<NodePtr>      nodes;

  gl::FrameBufferPtr      framebuffer;
  camera::CameraPtr       camera;

  gl::StatePtr            state;
  sg::MaterialPtr         material;
//  sg::TransformPtr        transform;  FIXME: probably only a matrix

  std::list<MeshPtr>      meshes;

};

}
}

#endif