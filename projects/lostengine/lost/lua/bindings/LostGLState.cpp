#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/gl/State.h"

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
          class_<lost::gl::State, boost::shared_ptr<lost::gl::State> >("State")
          .def(constructor<>())

          // server-side attributes
          .def_readwrite("alphaTest", &lost::gl::State::alphaTest)
          .def_readwrite("blend", &lost::gl::State::blend)
          .def_readwrite("blendSrc", &lost::gl::State::blendSrc)
          .def_readwrite("blendDest", &lost::gl::State::blendDest)
          .def_readwrite("depthTest", &lost::gl::State::depthTest)
          .def_readwrite("texture2D", &lost::gl::State::texture2D)

          .def_readwrite("clearColor", &lost::gl::State::clearColor)
          .def_readwrite("viewport", &lost::gl::State::viewport)
           
          // client-side attributes
          .def_readwrite("normalArray", &lost::gl::State::normalArray)
          .def_readwrite("textureCoordArray", &lost::gl::State::textureCoordArray)
          .def_readwrite("vertexArray", &lost::gl::State::vertexArray)
        ]
      ];
    }
  }
}
