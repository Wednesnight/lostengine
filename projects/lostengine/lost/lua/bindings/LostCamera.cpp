#include "lost/lua/bindings/LostCamera.h"
#include "lost/lua/lua.h"

#include "lost/camera/Camera.h"
#include "lost/camera/Camera2D.h"
#include "lost/camera/Camera3D.h"

using namespace boost;
using namespace luabind;
using namespace lost::camera;
using namespace lost::math;

namespace lost
{
  namespace lua
  {
    void LostCameraCamera(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("camera")
        [
          class_<Camera>("Camera")
            .def("viewport", (Rect&(Camera::*)(const Rect&))&Camera::viewport)
            .def("viewport", (Rect&(Camera::*)())&Camera::viewport)
            .def("projectionMatrix", &Camera::projectionMatrix),
          class_<Camera2D, Camera>("Camera2D")
            .scope
            [
              def("create", &Camera2D::create)
            ]
            ,
          class_<Camera3D, Camera>("Camera3D")
            .def("fovY", (float(Camera3D::*)()) &Camera3D::fovY)
            .def("fovY", (void(Camera3D::*)(const float)) &Camera3D::fovY)
            .def("position", (Vec3(Camera3D::*)()) &Camera3D::position)
            .def("position", (void(Camera3D::*)(const Vec3&)) &Camera3D::position)
            .def("direction", (Vec3(Camera3D::*)()) &Camera3D::direction)
            .def("direction", (void(Camera3D::*)(const Vec3&)) &Camera3D::direction)
            .def("stickToTarget", (bool(Camera3D::*)()) &Camera3D::stickToTarget)
            .def("stickToTarget", (void(Camera3D::*)(const bool)) &Camera3D::stickToTarget)
            .def("rotation", (Vec3(Camera3D::*)()) &Camera3D::rotation)
            .def("rotation", (void(Camera3D::*)(const Vec3&)) &Camera3D::rotation)
            .def("target", (Vec3(Camera3D::*)()) &Camera3D::target)
            .def("target", (void(Camera3D::*)(const Vec3&)) &Camera3D::target)
            .def("move", &Camera3D::move)
            .def("rotate", &Camera3D::rotate)
            .def("depth", (Vec2(Camera3D::*)()) &Camera3D::depth)
            .def("depth", (void(Camera3D::*)(const Vec2&)) &Camera3D::depth)
            .def("up", (Vec3(Camera3D::*)()) &Camera3D::up)
            .def("up", (void(Camera3D::*)(const Vec3&)) &Camera3D::up)
            .scope
            [
              def("create", &Camera3D::create)
            ]
        ]
      ];
    }

    void LostCamera(lua_State* state)
    {
      LostCameraCamera(state);
    }

  }
}
