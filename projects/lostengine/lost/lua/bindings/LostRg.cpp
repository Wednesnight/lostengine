#include "lost/lua/bindings/LostRg.h"
#include "lost/lua/lua.h"

#include "lost/rg/Node.h"
#include "lost/rg/Draw.h"
#include "lost/rg/DepthTest.h"
#include "lost/rg/Camera.h"


using namespace luabind;
using namespace lost::rg;

namespace lost
{
namespace lua
{
void LostRg(lua_State* state)
{
  module(state, "lost")
  [
    namespace_("rg")
    [
      class_<Node, NodePtr>("Node")
      .def_readwrite("name", &Node::name)
      .def_readwrite("active", &Node::active)
      .def("add", &Node::add)
      .scope
      [
        def("create", &Node::create)
      ],
      class_<Draw, NodePtr>("Draw")
      .def_readwrite("name", &Draw::name)
      .def_readwrite("active", &Draw::active)
      .def_readwrite("mesh", &Draw::mesh)
      .scope
      [
        def("create", &Draw::create)
      ],
      class_<Camera, NodePtr>("Camera")
      .def_readwrite("name", &Camera::name)
      .def_readwrite("active", &Camera::active)
      .def_readwrite("cam", &Camera::cam)
      .scope
      [
        def("create", (NodePtr(*)(camera::CameraPtr))&Camera::create),
        def("create", (NodePtr(*)(camera::Camera2DPtr))&Camera::create),
        def("create", (NodePtr(*)(camera::Camera3DPtr))&Camera::create)
      ],
      class_<DepthTest, NodePtr>("DepthTest")
      .def_readwrite("name", &DepthTest::name)
      .def_readwrite("active", &DepthTest::active)
      .def_readwrite("enable", &DepthTest::enable)
      .scope
      [
        def("create", &DepthTest::create)
      ]
      
    ]
  ];
}
}
}
