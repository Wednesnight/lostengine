#include "lost/lua/bindings/LostRg.h"
#include "lost/lua/lua.h"

#include "lost/gl/FrameBuffer.h"
#include "lost/gl/Context.h"
#include "lost/camera/Camera.h"
#include "lost/camera/Camera3D.h"
#include "lost/camera/Camera2D.h"

#include "lost/rg/Node.h"
#include "lost/rg/Draw.h"
#include "lost/rg/DepthTest.h"
#include "lost/rg/Camera.h"
#include "lost/rg/Clear.h"
#include "lost/rg/ClearColor.h"
#include "lost/rg/DefaultFrameBuffer.h"
#include "lost/rg/FrameBuffer.h"
#include "lost/rg/Blend.h"
#include "lost/rg/Scissor.h"
#include "lost/rg/ScissorRect.h"
#include "lost/rg/ScissorStack.h"

#include "lost/mesh/Line.h"
#include "lost/mesh/Quad.h"
#include "lost/mesh/ScaleGrid.h"
#include "lost/mesh/Rect.h"
#include "lost/mesh/Circular.h"
#include "lost/font/RenderedText.h"
#include <luabind/shared_ptr_converter.hpp>

#include <slub/slub.h>

using namespace luabind;
using namespace lost::rg;
using namespace lost::mesh;
using namespace lost::font;
using namespace slub;

namespace lost
{
  namespace lua
  {
    void LostRg(lua_State* state)
    {
/*
      package rg = package(state, "lost").package("rg");

      rg.clazz<Node>("Node")
        .method("add", &Node::add)
        .method("addFront", &Node::addFront)
        .method("remove", &Node::remove)
        .method("removeLast", &Node::removeLast)
        .method("process", &Node::process)
        .method("recursiveFindByName", &Node::recursiveFindByName)
        .method("print", &Node::print)
        .method("clear",&Node::clear)
        .field("name", &Node::name)
        .field("active", &Node::active)
        .function("create", &Node::create);

      rg.clazz<Draw>("Draw")
        .extends<Node>()
        .field("mesh", &Draw::mesh)
        .function("create", &Draw::create);

      rg.clazz<Camera>("Camera")
        .extends<Node>()
        .field("cam", &Camera::cam)
        .function("create", (rg::CameraPtr(*)(camera::CameraPtr))&Camera::create)
        .function("create", (rg::CameraPtr(*)(camera::Camera2DPtr))&Camera::create)
        .function("create", (rg::CameraPtr(*)(camera::Camera3DPtr))&Camera::create);

      rg.clazz<DepthTest>("DepthTest")
        .extends<Node>()
        .field("enable", &DepthTest::enable)
        .function("create", &DepthTest::create);

      rg.clazz<Clear>("Clear")
        .extends<Node>()
        .field("mask", &Clear::mask)
        .function("create", &Clear::create);

      rg.clazz<ClearColor>("ClearColor")
        .extends<Node>()
        .field("col", &ClearColor::col)
        .function("create", &ClearColor::create);

      rg.clazz<DefaultFrameBuffer>("DefaultFrameBuffer")
        .extends<Node>()
        .function("create", &DefaultFrameBuffer::create);

      rg.clazz<FrameBuffer>("FrameBuffer")
        .extends<Node>()
        .field("fb", &FrameBuffer::fb)
        .function("create", &FrameBuffer::create);

      rg.clazz<Blend>("Blend")
        .extends<Node>()
        .field("enable", &Blend::enable)
        .function("create", &Blend::create);

      rg.clazz<Scissor>("Scissor")
        .extends<Node>()
        .field("enable", &Scissor::enable)
        .function("create", &Scissor::create);

      rg.clazz<ScissorRect>("ScissorRect")
        .extends<Node>()
        .field("rect", &ScissorRect::rect)
        .function("create", &ScissorRect::create);

      rg.clazz<ScissorStack>("ScissorStack")
        .extends<Node>()
        .field("rect", &ScissorStack::rect)
        .field("clip", &ScissorStack::clip)
        .function("create", &ScissorStack::create);
*/
      module(state, "lost")
      [
        namespace_("rg")
        [
          class_<Node>("Node")
          .def_readwrite("name", &Node::name)
          .def_readwrite("active", &Node::active)
          .def("add", &Node::add)
          .def("addFront", &Node::addFront)
          .def("remove", &Node::remove)
          .def("removeLast", &Node::removeLast)
          .def("process", &Node::process)
          .def("recursiveFindByName", &Node::recursiveFindByName)
          .def("print", &Node::print)
          .def("clear",&Node::clear)
          .scope
          [
            def("create", &Node::create)
          ],
          class_<Draw, Node>("Draw")
          .def_readwrite("mesh", &Draw::mesh)
          .scope
          [
            def("create", &Draw::create)
          ],
          class_<Camera, Node>("Camera")
          .def_readwrite("cam", &Camera::cam)
          .scope
          [
            def("create", (rg::CameraPtr(*)(camera::CameraPtr))&Camera::create),
            def("create", (rg::CameraPtr(*)(camera::Camera2DPtr))&Camera::create),
            def("create", (rg::CameraPtr(*)(camera::Camera3DPtr))&Camera::create)
          ],
          class_<DepthTest,Node>("DepthTest")
          .def_readwrite("enable", &DepthTest::enable)
          .scope
          [
            def("create", &DepthTest::create)
          ],
          class_<Clear, Node>("Clear")
          .def_readwrite("mask", &Clear::mask)
          .scope
          [
            def("create", &Clear::create)
          ],
          class_<ClearColor, Node>("ClearColor")
          .def_readwrite("col", &ClearColor::col)
          .scope
          [
            def("create", &ClearColor::create)
          ],
          class_<DefaultFrameBuffer, Node>("DefaultFrameBuffer")
          .scope
          [
            def("create", &DefaultFrameBuffer::create)
          ],
          class_<FrameBuffer, Node>("FrameBuffer")
          .def_readwrite("fb", &FrameBuffer::fb)
          .scope
          [
            def("create", &FrameBuffer::create)
          ],
          class_<Blend, Node>("Blend")
          .def_readwrite("enable", &Blend::enable)
          .scope
          [
            def("create", &Blend::create)
          ],
          class_<Scissor, Node>("Scissor")
          .def_readwrite("enable", &Scissor::enable)
          .scope
          [
            def("create", &Scissor::create)
          ],
          class_<ScissorRect, Node>("ScissorRect")
          .def_readwrite("rect", &ScissorRect::rect)
          .scope
          [
            def("create", &ScissorRect::create)
          ],
          class_<ScissorStack, Node>("ScissorStack")
          .def_readwrite("rect", &ScissorStack::rect)
          .def_readwrite("clip", &ScissorStack::clip)
          .scope
          [
            def("create", &ScissorStack::create)
          ]         
        ]
      ];
    }
  }
}
