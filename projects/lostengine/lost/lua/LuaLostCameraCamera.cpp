#include "lost/lua/Luabindings.h"
#include "lost/camera/Camera.h"

using namespace luabind;
using namespace lost::camera;

LOST_LUA_EXPORT_BEGIN(bindLostCameraCamera)
  {
    module(state, "lost")
    [
      namespace_("camera")
      [
        class_<Camera, boost::shared_ptr<Camera> >("Camera")
        .def(constructor<>())
      ]
    ];
  }
LOST_LUA_EXPORT_END
