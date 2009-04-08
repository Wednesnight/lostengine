#include "lost/lua/lua.h"
#include "lost/application/Application.h"

#include "lost/lua/bindings/LostApplicationApplication.h"

using namespace boost;
using namespace luabind;
using namespace lost::application;
using namespace lost::event;

namespace lost
{
  namespace lua
  {
    void LostApplicationApplication(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<Application, ApplicationPtr>("Application")
           .def("quit", &Application::quit)
           .def("showMouse", &Application::showMouse)
           .def_readonly("loader", &Application::loader)
           .def_readonly("eventDispatcher", &Application::eventDispatcher)
        ]
      ];
    }
  }
}
