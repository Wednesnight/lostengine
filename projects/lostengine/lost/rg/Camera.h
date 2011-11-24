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

    static CameraPtr create(camera::CameraPtr inCam);

    void process(gl::ContextPtr& ctx);
  };
}
}

#endif