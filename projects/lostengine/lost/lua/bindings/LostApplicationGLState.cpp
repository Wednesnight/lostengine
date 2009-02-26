#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/application/gl/State.h"
#include "lost/application/gl/StateParam.h"

#include "lost/lua/bindings/LostApplicationGLState.h"

using namespace luabind;
using namespace lost::application::gl;

namespace lost
{
  namespace lua
  {
    void LostApplicationGLState(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          namespace_("gl")
          [
            class_<application::gl::State, SharedState>("State")
            .scope
            [
              def("create", (SharedState(*)(const SharedParam&))&application::gl::State::create),
              def("create", (SharedState(*)(const SharedParam&, const SharedParam&))&application::gl::State::create),
              def("create", (SharedState(*)(const SharedParam&, const SharedParam&, const SharedParam&))&application::gl::State::create),
              def("create", (SharedState(*)(const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&))&application::gl::State::create),
              def("create", (SharedState(*)(const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&))&application::gl::State::create),
              def("create", (SharedState(*)(const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&))&application::gl::State::create),
              def("create", (SharedState(*)(const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&))&application::gl::State::create)
            ],
            class_<application::gl::AlphaTest, SharedParam>("AlphaTest")
            .scope
            [
              def("create", (SharedParam(*)(bool))&application::gl::AlphaTest::create)
            ],

            class_<application::gl::DepthTest, SharedParam>("DepthTest")
            .scope
            [
              def("create", (SharedParam(*)(bool))&application::gl::DepthTest::create)
            ],

            class_<application::gl::Texture2D, SharedParam>("Texture2D")
            .scope
            [
              def("create", (SharedParam(*)(bool))&application::gl::Texture2D::create)
            ],

            class_<application::gl::Blend, SharedParam>("Blend")
            .scope
            [
              def("create", (SharedParam(*)(bool))&application::gl::Blend::create)
            ],
            
            class_<application::gl::NormalArray, SharedParam>("NormalArray")
            .scope
            [
              def("create", (SharedParam(*)(bool))&application::gl::NormalArray::create)
            ],
            
            class_<application::gl::VertexArray, SharedParam>("VertexArray")
            .scope
            [
              def("create", (SharedParam(*)(bool))&application::gl::VertexArray::create)
            ],
            
            class_<application::gl::TextureArray, SharedParam>("TextureArray")
            .scope
            [
              def("create", (SharedParam(*)(bool))&application::gl::TextureArray::create)
            ],
            
            class_<application::gl::ClearColor, SharedParam>("ClearColor")
            .scope
            [
              def("create", (SharedParam(*)(const lost::common::Color&))&application::gl::ClearColor::create)
            ],
            
            class_<application::gl::BlendFunc, SharedParam>("BlendFunc")
            .scope
            [
              def("create", (SharedParam(*)(GLenum, GLenum))&application::gl::BlendFunc::create)
            ]
          ]
        ]
      ];
    }
  }
}
