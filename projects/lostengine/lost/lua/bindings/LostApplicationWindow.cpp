#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/application/Window.h"

#include "lost/lua/bindings/LostApplicationWindow.h"

using namespace luabind;
using namespace lost::application;

namespace lost
{
  namespace lua
  {
    void LostApplicationWindow(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<Window, boost::shared_ptr<Window> >("Window")
            .def("open", &Window::open)
            .def("close", &Window::close)
            .def_readonly("context", &Window::context)
            .def_readonly("canvas", &Window::canvas)
            .def_readonly("params", &Window::params)
            .scope
            [
              def("create", &Window::create)
            ],
          class_<WindowParams, boost::shared_ptr<WindowParams> >("WindowParams")
            .def(constructor<const std::string&, const lost::math::Rect&>())
            .def_readwrite("caption", &WindowParams::caption)
            .def_readwrite("rect", &WindowParams::rect)
        ]
      ];
    }
  }
}
