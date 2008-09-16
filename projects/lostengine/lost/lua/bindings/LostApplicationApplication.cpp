#include "lost/lua/bindings/LostApplicationApplication.h"
#include "lost/application/Application.h"

using namespace luabind;
using namespace lost::application;
using namespace lost::event;

LOST_LUA_EXPORT_BEGIN(LostApplicationApplication)
{
  module(state, "lost")
  [
    namespace_("application")
    [
      class_<Application, EventDispatcher>("Application")
       .def(constructor<>())
       .def("quit", &Application::quit)
       .def("swapBuffers", &Application::swapBuffers)
       .def_readonly("loader", &Application::loader)
       .def_readonly("displayAttributes", &Application::displayAttributes)
       .def_readonly("context", &Application::context)
    ]
  ];
}
LOST_LUA_EXPORT_END
