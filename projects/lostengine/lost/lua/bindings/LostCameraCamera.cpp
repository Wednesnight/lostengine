#include "lost/lua/lua.h"
#include "lost/camera/Camera.h"
#include "lost/camera/Camera2D.h"
#include "lost/camera/Camera3D.h"

#include "lost/lua/bindings/LostCameraCamera.h"

using namespace boost;
using namespace luabind;
using namespace lost::camera;
using namespace lost::math;
using namespace lost::gl;

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
         class_<Camera, shared_ptr<Camera> >("Camera")
          .def(constructor<const shared_ptr<Context>&, const Rect&>())
          .def("apply", &Camera::apply)
          .def_readonly("viewport", &Camera::viewport),
         class_<Camera2D, shared_ptr<Camera2D>, Camera>("Camera2D")
          .def(constructor<const shared_ptr<Context>&, const Rect&>()),
         class_<Camera3D, shared_ptr<Camera3D>, Camera>("Camera3D")
          .def(constructor<const shared_ptr<Context>&, const Rect&>())
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
        ]
      ];
    }
  }
}
