#include "lost/lua/bindings/LostGLContext.h"
#include "lost/gl/Context.h"
#include "lost/gl/State.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/common/DisplayAttributes.h"

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
      .def("clear",  &Context::clear)
      .def("set2DProjection",  &Context::set2DProjection)
      .def("set3DProjection",  &Context::set3DProjection)
      .def("drawLine",  (void(Context::*)(const lost::math::Vec3&, const lost::math::Vec3&))&Context::drawLine)
      .def("drawLine",  (void(Context::*)(const lost::math::Vec2&, const lost::math::Vec2&))&Context::drawLine)
    ]
  ];
}
LOST_LUA_EXPORT_END
