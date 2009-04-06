#ifndef LOST_SG_NODE
#define LOST_SG_NODE

#include "lost/sg/Primitive.h"
#include "lost/camera/Camera.h"
#include "lost/gl/FrameBuffer.h"
#include "lost/gl/Context.h"
#include <list>

namespace lost
{
namespace sg
{

struct Node;
typedef boost::shared_ptr<Node> NodePtr;
  
struct Node : public Primitive
{
  Node();
  virtual ~Node();

  void add(PrimitivePtr primitive);

  std::list<PrimitivePtr> primitives;
  camera::CameraPtr       camera;
  gl::FrameBufferPtr      framebuffer;
};

}
}

#endif