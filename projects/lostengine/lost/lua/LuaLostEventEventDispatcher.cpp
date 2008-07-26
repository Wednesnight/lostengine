#include "lost/lua/Luabindings.h"
#include "lost/event/EventDispatcher.h"
#include "lost/common/Logger.h"

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
  
void addEventListener(object dispatcher, const string& type, object func)
{
  if(luabind::type(func) == LUA_TNIL) { throw runtime_error("can't register NIL lua callback function"); }
  EventDispatcher* disp = object_cast<EventDispatcher*>(dispatcher);
  disp->addEventListener(type, LuaHandlerFunction(func));
}

void addEventListener(object dispatcher, const string& type, object obj, object method)
{
  if(luabind::type(obj) == LUA_TNIL) { throw runtime_error("can't register lua callback method with NIL object"); }
  if(luabind::type(method) == LUA_TNIL) { throw runtime_error("can't register NIL lua callback method"); }
  EventDispatcher* disp = object_cast<EventDispatcher*>(dispatcher);
  disp->addEventListener(type, LuaHandlerMethod(obj, method));
}
  
void bindLostEventEventDispatcher(lost::lua::State& state)
{
  module(state, "lost")
  [
    namespace_("event")
    [
      class_<EventDispatcher>("EventDispatcher")
        .def(constructor<>())
        .def("addEventListener", (void(*)(object, const string&, object))&addEventListener)
        .def("addEventListener", (void(*)(object, const string&, object, object))&addEventListener)
    ]
  ];
}  
}
}

