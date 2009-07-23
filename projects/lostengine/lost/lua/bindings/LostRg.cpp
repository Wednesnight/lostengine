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
#include "lost/rg/Blend.h"

#include "lost/mesh/Line.h"
#include "lost/mesh/Quad.h"
#include "lost/mesh/Rect.h"
#include "lost/mesh/Circular.h"
#include "lost/font/RenderedText.h"

using namespace luabind;
using namespace lost::rg;
using namespace lost::mesh;
using namespace lost::font;

namespace lost
{
  namespace lua
  {
    // FIXME: this gay stuff has to be specified for each possible mesh type since luabind is unable to downcast smart_ptr types
    #define NodePtrDowncastDraw(type) NodePtr DrawCreate##type(type _##type) { return Draw::create(_##type); }
    NodePtrDowncastDraw(Line2DPtr);
    NodePtrDowncastDraw(Quad2DPtr);
    NodePtrDowncastDraw(Rect2DPtr);
    NodePtrDowncastDraw(RoundedRect2DPtr);
    NodePtrDowncastDraw(FilledRect2DPtr);
    NodePtrDowncastDraw(FilledRoundedRect2DPtr);
    NodePtrDowncastDraw(Circle2DPtr);
    NodePtrDowncastDraw(Ellipse2DPtr);
    NodePtrDowncastDraw(FilledCircle2DPtr);
    NodePtrDowncastDraw(FilledEllipse2DPtr);
    NodePtrDowncastDraw(RenderedTextPtr);
    #undef NodePtrDowncastDraw

    DrawPtr castDraw(NodePtr node)
    {
        return dynamic_pointer_cast<rg::Draw>(node);
    }

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
          .def("addFront", &Node::addFront)
          .def("remove", &Node::remove)
          .def("process", &Node::process)
          .def("recursiveFindByName", &Node::recursiveFindByName)
          .scope
          [
            def("create", &Node::create)
          ],
          class_<Draw, Node, NodePtr>("Draw")
          .def_readwrite("mesh", &Draw::mesh)
          .scope
          [
            def("create", &Draw::create),
            // FIXME: this gay stuff has to be specified for each possible mesh type since luabind is unable to downcast smart_ptr types
            def("create", &DrawCreateRect2DPtr),
            def("create", &DrawCreateRoundedRect2DPtr),
            def("create", &DrawCreateFilledRect2DPtr),
            def("create", &DrawCreateFilledRoundedRect2DPtr),
            def("create", &DrawCreateCircle2DPtr),
            def("create", &DrawCreateEllipse2DPtr),
            def("create", &DrawCreateFilledCircle2DPtr),
            def("create", &DrawCreateFilledEllipse2DPtr),
            def("create", &DrawCreateLine2DPtr),
            def("create", &DrawCreateQuad2DPtr),
            def("create", &DrawCreateRenderedTextPtr),
            def("cast", castDraw)
          ],
          class_<Camera, Node, NodePtr>("Camera")
          .def_readwrite("cam", &Camera::cam)
          .scope
          [
            def("create", (NodePtr(*)(camera::CameraPtr))&Camera::create),
            def("create", (NodePtr(*)(camera::Camera2DPtr))&Camera::create),
            def("create", (NodePtr(*)(camera::Camera3DPtr))&Camera::create)
          ],
          class_<DepthTest,Node,  NodePtr>("DepthTest")
          .def_readwrite("enable", &DepthTest::enable)
          .scope
          [
            def("create", &DepthTest::create)
          ],
          class_<Clear, Node, NodePtr>("Clear")
          .def_readwrite("mask", &Clear::mask)
          .scope
          [
            def("create", &Clear::create)
          ],
          class_<ClearColor, Node, NodePtr>("ClearColor")
          .def_readwrite("col", &ClearColor::col)
          .scope
          [
            def("create", &ClearColor::create)
          ],
          class_<DefaultFrameBuffer, Node, NodePtr>("DefaultFrameBuffer")
          .scope
          [
            def("create", &DefaultFrameBuffer::create)
          ],
          class_<FrameBuffer, Node, NodePtr>("FrameBuffer")
          .def_readwrite("fb", &FrameBuffer::fb)
          .scope
          [
            def("create", &FrameBuffer::create)
          ],
          class_<Blend, Node, NodePtr>("Blend")
          .def_readwrite("enable", &Blend::enable)
          .scope
          [
            def("create", &Blend::create)
          ]
         
        ]
      ];
    }
  }
}
