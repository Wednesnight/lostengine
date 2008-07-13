#include "lost/lua/Luabindings.h"
#include "lost/event/EventDispatcher.h"

using namespace std;
using namespace boost;
using namespace luabind;
using namespace lost::event;


namespace lost
{
namespace lua
{
struct LuaHandlerFunction
{
  luabind::object func;
  
  LuaHandlerFunction(luabind::object inFunction) : func(inFunction) {}
  
  void operator()(EventPtr event)
  {
    luabind::call_function<void>(func, event);
  }
};

struct LuaHandlerMethod
{
  luabind::object obj;
  luabind::object method;
  
  LuaHandlerMethod(luabind::object inObj, luabind::object inMethod)
  : obj(inObj), method(inMethod) {}
  
  void operator()(EventPtr event)
  {
    luabind::call_function<void>(method, obj, event);
  }
};
  
void addEventListener(luabind::object dispatcher, const std::string& type, luabind::object func)
{
  EventDispatcher* disp = object_cast<EventDispatcher*>(dispatcher);
  disp->addEventListener(type, LuaHandlerFunction(func));
}

void bindLostEventEventDispatcher(lost::lua::State& state)
{
  module(state, "lost")
  [
    namespace_("event")
    [
      class_<EventDispatcher>("EventDispatcher")
        .def(constructor<>())
        .def("addEventListener", &addEventListener)
    ]
  ];
}  
}
}

