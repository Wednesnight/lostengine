#include "lost/lua/Luabindings.h"
#include "lost/application/Application.h"

using namespace std;
using namespace boost;
using namespace luabind;
using namespace lost::application;
using namespace lost::event;

namespace lost
{
namespace lua
{
void bindLostApplicationApplication(lost::lua::State& state)
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
}
}