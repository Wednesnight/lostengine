#include "lost/rg/Camera.h"
#include "lost/camera/Camera.h"
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

CameraPtr Camera::create(camera::CameraPtr inCam)
{
  return CameraPtr(new Camera(inCam));
}

void Camera::process(gl::ContextPtr ctx)
{
  ctx->camera(cam); 
}

}
}