#include "lost/lua/bindings/LostCamera.h"
#include "lost/camera/Camera.h"
#include "lost/camera/Camera2D.h"
#include "lost/camera/Camera3D.h"

#include <slub/slub.h>

using namespace lost::camera;
using namespace lost::math;

namespace lost
{
  namespace lua
  {
    void LostCameraCamera(lua_State* state)
    {
      slub::package camera = slub::package(state, "lost").package("camera");

      camera.clazz<Camera>("Camera")
        .method("viewport", (Rect&(Camera::*)(const Rect&))&Camera::viewport)
        .method("viewport", (Rect&(Camera::*)())&Camera::viewport)
        .method("viewMatrix", &Camera::viewMatrix)
        .method("projectionMatrix", &Camera::projectionMatrix);

      camera.clazz<Camera2D>("Camera2D")
        .extends<Camera>()
        .function("create", &Camera2D::create);

      camera.clazz<Camera3D>("Camera3D")
        .extends<Camera>()
        .method("fovY", (float(Camera3D::*)()) &Camera3D::fovY)
        .method("fovY", (void(Camera3D::*)(const float)) &Camera3D::fovY)
        .method("position", (Vec3(Camera3D::*)()) &Camera3D::position)
        .method("position", (void(Camera3D::*)(const Vec3&)) &Camera3D::position)
        .method("direction", (Vec3(Camera3D::*)()) &Camera3D::direction)
        .method("direction", (void(Camera3D::*)(const Vec3&)) &Camera3D::direction)
        .method("stickToTarget", (bool(Camera3D::*)()) &Camera3D::stickToTarget)
        .method("stickToTarget", (void(Camera3D::*)(const bool)) &Camera3D::stickToTarget)
        .method("rotation", (Vec3(Camera3D::*)()) &Camera3D::rotation)
        .method("rotation", (void(Camera3D::*)(const Vec3&)) &Camera3D::rotation)
        .method("target", (Vec3(Camera3D::*)()) &Camera3D::target)
        .method("target", (void(Camera3D::*)(const Vec3&)) &Camera3D::target)
        .method("move", &Camera3D::move)
        .method("rotate", &Camera3D::rotate)
        .method("depth", (Vec2(Camera3D::*)()) &Camera3D::depth)
        .method("depth", (void(Camera3D::*)(const Vec2&)) &Camera3D::depth)
        .method("up", (Vec3(Camera3D::*)()) &Camera3D::up)
        .method("up", (void(Camera3D::*)(const Vec3&)) &Camera3D::up)
        .function("create", &Camera3D::create);
    }

    void LostCamera(lua_State* state)
    {
      LostCameraCamera(state);
    }

  }
}
