#ifndef LOST_SG_BATCH
#define LOST_SG_BATCH

#include "lost/sg/Primitive.h"
#include "lost/camera/Camera.h"
#include "lost/gl/FrameBuffer.h"
#include "lost/gl/Context.h"
#include <list>

namespace lost
{
namespace sg
{

struct Batch;
typedef boost::shared_ptr<Batch> BatchPtr;
  
struct Batch : public Primitive
{
  Batch();
  virtual ~Batch();

  void add(PrimitivePtr primitive);
  void render(gl::ContextPtr ctx);

  std::list<PrimitivePtr> primitives;
  camera::CameraPtr       camera;
  gl::FrameBuffer         framebuffer;
};

}
}

#endif