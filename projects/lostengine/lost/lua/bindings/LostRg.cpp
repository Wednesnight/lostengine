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
#include "lost/rg/Scissor.h"
#include "lost/rg/ScissorRect.h"

#include "lost/mesh/Line.h"
#include "lost/mesh/Quad.h"
#include "lost/mesh/ScaleGrid.h"
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
    NodePtrDowncastDraw(ScaleGrid2DPtr);
    #undef NodePtrDowncastDraw

    template <typename ResultType, typename NodeType>
    lost::shared_ptr<ResultType> castNode(lost::shared_ptr<NodeType> node)
    {
        return dynamic_pointer_cast<ResultType>(node);
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
          .def("print", &Node::print)
          .scope
          [
            def("create", &Node::create)
          ],
          class_<Draw, Node, DrawPtr>("Draw")
          .def_readwrite("mesh", &Draw::mesh)
          .def("asNodePtr", &castNode<Node, Draw>)
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
            def("create", &DrawCreateScaleGrid2DPtr),
            def("create", &DrawCreateRenderedTextPtr),
            def("cast", &castNode<Draw, Node>)
          ],
          class_<Camera, Node, CameraPtr>("Camera")
          .def_readwrite("cam", &Camera::cam)
          .def("asNodePtr", &castNode<Node, Camera>)
          .scope
          [
            def("create", (NodePtr(*)(camera::CameraPtr))&Camera::create),
            def("create", (NodePtr(*)(camera::Camera2DPtr))&Camera::create),
            def("create", (NodePtr(*)(camera::Camera3DPtr))&Camera::create),
            def("cast", &castNode<Camera, Node>)
          ],
          class_<DepthTest,Node,  DepthTestPtr>("DepthTest")
          .def_readwrite("enable", &DepthTest::enable)
          .def("asNodePtr", &castNode<Node, DepthTest>)
          .scope
          [
            def("create", &DepthTest::create),
            def("cast", &castNode<DepthTest, Node>)
          ],
          class_<Clear, Node, ClearPtr>("Clear")
          .def_readwrite("mask", &Clear::mask)
          .def("asNodePtr", &castNode<Node, Clear>)
          .scope
          [
            def("create", &Clear::create),
            def("cast", &castNode<Clear, Node>)
          ],
          class_<ClearColor, Node, ClearColorPtr>("ClearColor")
          .def_readwrite("col", &ClearColor::col)
          .def("asNodePtr", &castNode<Node, ClearColor>)
          .scope
          [
            def("create", &ClearColor::create),
            def("cast", &castNode<ClearColor, Node>)
          ],
          class_<DefaultFrameBuffer, Node, DefaultFrameBufferPtr>("DefaultFrameBuffer")
          .def("asNodePtr", &castNode<Node, DefaultFrameBuffer>)
          .scope
          [
            def("create", &DefaultFrameBuffer::create),
            def("cast", &castNode<DefaultFrameBuffer, Node>)
          ],
          class_<FrameBuffer, Node, FrameBufferPtr>("FrameBuffer")
          .def_readwrite("fb", &FrameBuffer::fb)
          .def("asNodePtr", &castNode<Node, FrameBuffer>)
          .scope
          [
            def("create", &FrameBuffer::create),
            def("cast", &castNode<FrameBuffer, Node>)
          ],
          class_<Blend, Node, BlendPtr>("Blend")
          .def_readwrite("enable", &Blend::enable)
          .def("asNodePtr", &castNode<Node, Blend>)
          .scope
          [
            def("create", &Blend::create),
            def("cast", &castNode<Blend, Node>)
          ],
          class_<Scissor, Node, ScissorPtr>("Scissor")
          .def_readwrite("enable", &Scissor::enable)
          .def("asNodePtr", &castNode<Node, Scissor>)
          .scope
          [
            def("create", &Scissor::create),
            def("cast", &castNode<Scissor, Node>)
          ],
          class_<ScissorRect, Node, ScissorRectPtr>("ScissorRect")
          .def_readwrite("rect", &ScissorRect::rect)
          .def("asNodePtr", &castNode<Node, ScissorRect>)
          .scope
          [
            def("create", &ScissorRect::create),
            def("cast", &castNode<ScissorRect, Node>)
          ]
         
        ]
      ];
    }
  }
}
