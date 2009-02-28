#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/gl/State.h"
#include "lost/gl/StateParam.h"

#include "lost/lua/bindings/LostGLState.h"

using namespace luabind;
using namespace lost::gl;

namespace lost
{
  namespace lua
  {
    void LostGLState(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<gl::State, SharedState>("State")
          .def("param", &gl::State::param)
          .scope
          [
            def("create", (SharedState(*)())&gl::State::create),
            def("create", (SharedState(*)(const SharedParam&))&gl::State::create),
            def("create", (SharedState(*)(const SharedParam&, const SharedParam&))&gl::State::create),
            def("create", (SharedState(*)(const SharedParam&, const SharedParam&, const SharedParam&))&gl::State::create),
            def("create", (SharedState(*)(const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&))&gl::State::create),
            def("create", (SharedState(*)(const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&))&gl::State::create),
            def("create", (SharedState(*)(const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&))&gl::State::create),
            def("create", (SharedState(*)(const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&))&gl::State::create)
          ],
          class_<gl::AlphaTest, SharedParam>("AlphaTest")
          .scope
          [
            def("create", (SharedParam(*)(bool))&gl::AlphaTest::create)
          ],

          class_<gl::DepthTest, SharedParam>("DepthTest")
          .scope
          [
            def("create", (SharedParam(*)(bool))&gl::DepthTest::create)
          ],

          class_<gl::Texture2D, SharedParam>("Texture2D")
          .scope
          [
            def("create", (SharedParam(*)(bool))&gl::Texture2D::create)
          ],

          class_<gl::Blend, SharedParam>("Blend")
          .scope
          [
            def("create", (SharedParam(*)(bool))&gl::Blend::create)
          ],
          
          class_<gl::Scissor, SharedParam>("Scissor")
          .scope
          [
            def("create", (SharedParam(*)(bool))&gl::Scissor::create)
          ],
         
         class_<gl::NormalArray, SharedParam>("NormalArray")
          .scope
          [
            def("create", (SharedParam(*)(bool))&gl::NormalArray::create)
          ],
          
          class_<gl::VertexArray, SharedParam>("VertexArray")
          .scope
          [
            def("create", (SharedParam(*)(bool))&gl::VertexArray::create)
          ],
          
          class_<gl::TextureArray, SharedParam>("TextureArray")
          .scope
          [
            def("create", (SharedParam(*)(bool))&gl::TextureArray::create)
          ],
          
          class_<gl::ClearColor, SharedParam>("ClearColor")
          .scope
          [
            def("create", (SharedParam(*)(const lost::common::Color&))&gl::ClearColor::create)
          ],
          
          class_<gl::BlendFunc, SharedParam>("BlendFunc")
          .scope
          [
            def("create", (SharedParam(*)(GLenum, GLenum))&gl::BlendFunc::create)
          ],

          class_<gl::ScissorBox, SharedParam>("ScissorBox")
          .scope
          [
            def("create", (SharedParam(*)(const math::Rect&))&gl::ScissorBox::create)
          ]
        ]
      ];
    }
  }
}
