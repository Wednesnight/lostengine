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
            .def_readonly("context", &Window::context),
          class_<WindowParams, boost::shared_ptr<WindowParams> >("WindowParams")
            .def(constructor<const std::string&, const lost::math::Vec2&, const lost::math::Vec2&>()) 
        ]
      ];
    }
  }
}
