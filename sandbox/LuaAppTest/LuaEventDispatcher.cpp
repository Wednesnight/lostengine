#include "luabindings.h"
#include "EventDispatcher.h"

using namespace std;
using namespace luabind;

void bindEventDispatcher(lost::lua::State& state)
{
  module(state)
  [
    class_<EventDispatcher>("EventDispatcher")
      .def(constructor<>())
      .def("dispatchEvent", &EventDispatcher::dispatchEvent)
      .def("addEventListener", (void(EventDispatcher::*)(const std::string&, const luabind::object&)) &EventDispatcher::addEventListener)
      .def("doDispatch", &EventDispatcher::doDispatch)
  ];
}
