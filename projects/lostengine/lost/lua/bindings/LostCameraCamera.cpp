#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/camera/Camera.h"

#include "lost/lua/bindings/LostCameraCamera.h"

using namespace luabind;
using namespace lost::camera;

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
          class_<Camera, boost::shared_ptr<Camera> >("Camera")
          .def(constructor<>())
          .def("depth", (math::Vec2&(Camera::*)()) &Camera::depth)
          .def("depth", (void(Camera::*)(const math::Vec2&)) &Camera::depth)
          .def("fovY", (float&(Camera::*)()) &Camera::fovY)
          .def("fovY", (void(Camera::*)(const float)) &Camera::fovY)
          .def("position", (math::Vec3(Camera::*)()) &Camera::position)
          .def("position", (void(Camera::*)(const math::Vec3&)) &Camera::position)
          .def("direction", (math::Vec3(Camera::*)()) &Camera::direction)
          .def("direction", (void(Camera::*)(const math::Vec3&)) &Camera::direction)
          .def("stickToTarget", (bool(Camera::*)()) &Camera::stickToTarget)
          .def("stickToTarget", (void(Camera::*)(const bool)) &Camera::stickToTarget)
          .def("rotation", (math::Vec3&(Camera::*)()) &Camera::rotation)
          .def("rotation", (void(Camera::*)(const math::Vec3&)) &Camera::rotation)
          .def("target", (math::Vec3(Camera::*)()) &Camera::target)
          .def("target", (void(Camera::*)(const math::Vec3&)) &Camera::target)
          .def("move", &Camera::move)
          .def("rotate", &Camera::rotate)
        ]
      ];
    }
  }
}
