#include "lost/rg/Camera.h"
#include "lost/camera/Camera.h"
#include "lost/camera/Camera2D.h"
#include "lost/camera/Camera3D.h"
#include "lost/gl/Context.h"

namespace lost
{
namespace rg
{

Camera::Camera(camera::CameraPtr inCam)
: cam(inCam)
{
  name = "camera"; 
}

NodePtr Camera::create(camera::CameraPtr inCam)
{
  return CameraPtr(new Camera(inCam));
}

NodePtr Camera::create(camera::Camera2DPtr inCam)
{
  return CameraPtr(new Camera(inCam));
}

NodePtr Camera::create(camera::Camera3DPtr inCam)
{
  return CameraPtr(new Camera(inCam));
}

void Camera::process(gl::ContextPtr ctx)
{
  ctx->camera(cam); 
}

}
}