#ifndef LOST_RG_CAMERA_H
#define LOST_RG_CAMERA_H

#include "lost/rg/Node.h"
#include "lost/camera/forward.h"
#include "lost/rg/forward.h"

namespace lost
{
namespace rg
{

  /** sets the camera to the camera instance provided in the constructor.
   * does not process children.
   */
  struct Camera : public Node
  {
    camera::CameraPtr cam;

    Camera(camera::CameraPtr inCam);

    // the fact that all create functions return a NodePtr is another hack for luabind because of the missing downcast
    static CameraPtr create(camera::CameraPtr inCam);
    // the following two create functions are a hack because luabind doesn't perform
    // an automatic downcast. Remove these as soon as luabind supports downcast.
    static CameraPtr create(camera::Camera2DPtr inCam);
    static CameraPtr create(camera::Camera3DPtr inCam);

    void process(gl::ContextPtr& ctx);
  };
}
}

#endif