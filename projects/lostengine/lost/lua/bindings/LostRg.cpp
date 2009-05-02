#include "lost/lua/bindings/LostRg.h"
#include "lost/lua/lua.h"

#include "lost/rg/Node.h"
#include "lost/rg/Draw.h"
#include "lost/rg/DepthTest.h"
#include "lost/rg/Camera.h"
#include "lost/rg/Clear.h"
#include "lost/rg/ClearColor.h"
#include "lost/rg/DefaultFrameBuffer.h"
#include "lost/rg/FrameBuffer.h"


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
      ],
      class_<Clear, NodePtr>("Clear")
      .def_readwrite("name", &Clear::name)
      .def_readwrite("active", &Clear::active)
      .def_readwrite("mask", &Clear::mask)
      .scope
      [
        def("create", &Clear::create)
      ],
      class_<ClearColor, NodePtr>("ClearColor")
      .def_readwrite("name", &ClearColor::name)
      .def_readwrite("active", &ClearColor::active)
      .def_readwrite("col", &ClearColor::col)
      .scope
      [
        def("create", &ClearColor::create)
      ],
      class_<DefaultFrameBuffer, NodePtr>("DefaultFrameBuffer")
      .def_readwrite("name", &DefaultFrameBuffer::name)
      .def_readwrite("active", &DefaultFrameBuffer::active)
      .scope
      [
        def("create", &DefaultFrameBuffer::create)
      ],
      class_<FrameBuffer, NodePtr>("FrameBuffer")
      .def_readwrite("name", &FrameBuffer::name)
      .def_readwrite("active", &FrameBuffer::active)
      .def_readwrite("fb", &FrameBuffer::fb)
      .scope
      [
        def("create", &FrameBuffer::create)
      ]
      
    ]
  ];
}
}
}
