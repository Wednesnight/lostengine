#include "lost/lua/bindings/LostGLState.h"
#include "lost/gl/State.h"

using namespace luabind;
using namespace lost::gl;

LOST_LUA_EXPORT_BEGIN(LostGLState)
{
  module(state, "lost")
  [
    namespace_("gl")
    [
      class_<lost::gl::State, boost::shared_ptr<lost::gl::State> >("State")
      .def(constructor<>())

      // server-side attributes
      .def_readwrite("alphaTest", &lost::gl::State::alphaTest)
      .def_readwrite("depthTest", &lost::gl::State::depthTest)
      .def_readwrite("texture2D", &lost::gl::State::texture2D)
       
      // client-side attributes
      .def_readwrite("normalArray", &lost::gl::State::normalArray)
      .def_readwrite("vertexArray", &lost::gl::State::vertexArray)
    ]
  ];
}
LOST_LUA_EXPORT_END
