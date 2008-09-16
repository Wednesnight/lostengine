#include "lost/lua/bindings/LostGLContext.h"
#include "lost/gl/Context.h"
#include "lost/gl/State.h"

using namespace luabind;
using namespace lost::gl;

LOST_LUA_EXPORT_BEGIN(LostGLContext)
{
  module(state, "lost")
  [
    namespace_("gl")
    [
      class_<Context, boost::shared_ptr<Context> >("Context")
      .def(constructor<>())
      .def("newState",  &Context::newState)
      .def("copyState", &Context::copyState)
      .def("pushState", (void(Context::*)())&Context::pushState)
      .def("pushState", (void(Context::*)(const boost::shared_ptr<lost::gl::State>&))&Context::pushState)
      .def("popState",  &Context::popState)
      .def("drawLine",  &Context::drawLine)
    ]
  ];
}
LOST_LUA_EXPORT_END
