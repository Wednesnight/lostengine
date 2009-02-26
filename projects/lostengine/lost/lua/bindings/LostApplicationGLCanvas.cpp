#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/application/gl/Canvas.h"

#include "lost/lua/bindings/LostApplicationGLCanvas.h"

using namespace boost;
using namespace luabind;
using namespace lost::application::gl;
using namespace lost::math;
using namespace lost::camera;
using namespace lost::gl;

namespace lost
{
  namespace lua
  {
    void LostApplicationGLCanvas(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          namespace_("gl")
          [
            class_<Canvas, boost::shared_ptr<Canvas> >("Canvas")
              .def(constructor<const shared_ptr<Context>&, const Rect&>())
              .def(constructor<const shared_ptr<Context>&, const shared_ptr<Camera>&>())
              .def_readonly("context", &Canvas::context)
              .def_readonly("camera", &Canvas::camera)
              .def("clear", &Canvas::clear)
              .def("setColor", &Canvas::setColor)
              .def("drawPoint", &Canvas::drawPoint)
              .def("drawLine", (void(Canvas::*)(const Vec2&, const Vec2&))&Canvas::drawLine)
              .def("drawLine", (void(Canvas::*)(const Vec3&, const Vec3&))&Canvas::drawLine)
              .def("drawRectOutline", &Canvas::drawRectOutline)
              .def("drawRectFilled", &Canvas::drawRectFilled)
              .def("drawRectTextured", (void(Canvas::*)(const Rect&, shared_ptr<const Texture>, const Vec2&, const Vec2&, const Vec2&, const Vec2&, bool))&Canvas::drawRectTextured)
              .def("drawRectTextured", (void(Canvas::*)(const Rect&, shared_ptr<const Texture>, bool))&Canvas::drawRectTextured)
              .def("drawMesh2D", &Canvas::drawMesh2D)
              .def("drawMesh3D", &Canvas::drawMesh3D)
              .def("drawAABB", &Canvas::drawAABB)
              .def("drawAxes", &Canvas::drawAxes)
              .def("writeScreenshot", &Canvas::writeScreenshot)
          ]
        ]
      ];
    }
  }
}
