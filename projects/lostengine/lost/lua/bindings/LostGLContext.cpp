#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/gl/Context.h"
#include "lost/gl/State.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/Rect.h"
#include "lost/common/DisplayAttributes.h"

#include "lost/lua/bindings/LostGLContext.h"

using namespace luabind;
using namespace lost::gl;

namespace lost
{
  namespace lua
  {
    void LostGLContext(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<Context, boost::shared_ptr<Context> >("Context")
          .def(constructor<boost::shared_ptr<lost::common::DisplayAttributes> >())
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
          .def("setColor",  &Context::setColor)
          .def("drawRectOutline",  &Context::drawRectOutline)
          .def("drawRectFilled",  &Context::drawRectFilled)
          .def("drawRectTextured",  &Context::drawRectTextured)
        ]
      ];
    }
  }
}
