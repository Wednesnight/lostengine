/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/lua/bindings/LostRg.h"
#include "lost/lua/lostlua.h"

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

#include <slub/slub.h>

using namespace lost::rg;
using namespace lost::mesh;
using namespace lost::font;

namespace lost
{
  namespace lua
  {
    void LostRg(lua_State* state)
    {
      slub::package rg = slub::package(state, "lost").package("rg");

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
    }
  }
}
