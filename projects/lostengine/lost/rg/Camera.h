#ifndef LOST_RG_CAMERA_H
#define LOST_RG_CAMERA_H

#include "lost/rg/Node.h"

namespace lost
{
namespace camera
{
  struct Camera;
  typedef lost::shared_ptr<Camera> CameraPtr;
  struct Camera2D;
  typedef lost::shared_ptr<Camera2D> Camera2DPtr;
  struct Camera3D;
  typedef lost::shared_ptr<Camera3D> Camera3DPtr;
}
namespace rg
{

  struct Camera;
  typedef lost::shared_ptr<Camera> CameraPtr;
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

    void process(gl::ContextPtr ctx);
  };
}
}

#endif