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


using namespace luabind;
using namespace lost::rg;
using namespace lost::mesh;
using namespace lost::font;

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
