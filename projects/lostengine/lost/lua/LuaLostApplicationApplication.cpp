#include "lost/lua/Luabindings.h"
#include "lost/application/Application.h"

using namespace luabind;
using namespace lost::application;
using namespace lost::event;

LOST_LUA_EXPORT_BEGIN(LuaLostApplicationApplication)
{
  module(state, "lost")
  [
    namespace_("event")
    [
      class_<Application, EventDispatcher>("Application")
       .def(constructor<>())
       .def("quit", &Application::quit)
       .def_readonly("loader", &Application::loader)
    ]
  ];
}
LOST_LUA_EXPORT_END
