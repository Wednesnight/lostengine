#ifndef LOST_CAMERA_FORWARD_H
#define LOST_CAMERA_FORWARD_H

namespace lost
{
namespace camera
{ 
struct Camera; typedef lost::shared_ptr<Camera> CameraPtr; 
struct Camera2D; typedef lost::shared_ptr<Camera2D> Camera2DPtr; 
struct Camera3D; typedef lost::shared_ptr<Camera3D> Camera3DPtr; 
}
}

#endif