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
          class_<gl::State, StatePtr>("State")
          .def("param", &gl::State::param)
          .scope
          [
            def("create", (StatePtr(*)())&gl::State::create),
            def("create", (StatePtr(*)(const ParamPtr&))&gl::State::create),
            def("create", (StatePtr(*)(const ParamPtr&, const ParamPtr&))&gl::State::create),
            def("create", (StatePtr(*)(const ParamPtr&, const ParamPtr&, const ParamPtr&))&gl::State::create),
            def("create", (StatePtr(*)(const ParamPtr&, const ParamPtr&, const ParamPtr&, const ParamPtr&))&gl::State::create),
            def("create", (StatePtr(*)(const ParamPtr&, const ParamPtr&, const ParamPtr&, const ParamPtr&, const ParamPtr&))&gl::State::create),
            def("create", (StatePtr(*)(const ParamPtr&, const ParamPtr&, const ParamPtr&, const ParamPtr&, const ParamPtr&, const ParamPtr&))&gl::State::create),
            def("create", (StatePtr(*)(const ParamPtr&, const ParamPtr&, const ParamPtr&, const ParamPtr&, const ParamPtr&, const ParamPtr&, const ParamPtr&))&gl::State::create)
          ],
          class_<gl::AlphaTest, ParamPtr>("AlphaTest")
          .scope
          [
            def("create", (ParamPtr(*)(bool))&gl::AlphaTest::create)
          ],

          class_<gl::DepthTest, ParamPtr>("DepthTest")
          .scope
          [
            def("create", (ParamPtr(*)(bool))&gl::DepthTest::create)
          ],

          class_<gl::Texture2D, ParamPtr>("Texture2D")
          .scope
          [
            def("create", (ParamPtr(*)(bool))&gl::Texture2D::create)
          ],

          class_<gl::Blend, ParamPtr>("Blend")
          .scope
          [
            def("create", (ParamPtr(*)(bool))&gl::Blend::create)
          ],
          
          class_<gl::Scissor, ParamPtr>("Scissor")
          .scope
          [
            def("create", (ParamPtr(*)(bool))&gl::Scissor::create)
          ],
         
         class_<gl::NormalArray, ParamPtr>("NormalArray")
          .scope
          [
            def("create", (ParamPtr(*)(bool))&gl::NormalArray::create)
          ],
          
          class_<gl::VertexArray, ParamPtr>("VertexArray")
          .scope
          [
            def("create", (ParamPtr(*)(bool))&gl::VertexArray::create)
          ],
          
          class_<gl::TextureArray, ParamPtr>("TextureArray")
          .scope
          [
            def("create", (ParamPtr(*)(bool))&gl::TextureArray::create)
          ],
          
          class_<gl::ClearColor, ParamPtr>("ClearColor")
          .scope
          [
            def("create", (ParamPtr(*)(const lost::common::Color&))&gl::ClearColor::create)
          ],
          
          class_<gl::BlendFunc, ParamPtr>("BlendFunc")
          .scope
          [
            def("create", (ParamPtr(*)(GLenum, GLenum))&gl::BlendFunc::create)
          ],

          class_<gl::ScissorBox, ParamPtr>("ScissorBox")
          .scope
          [
            def("create", (ParamPtr(*)(const math::Rect&))&gl::ScissorBox::create)
          ]
        ]
      ];
    }
  }
}
