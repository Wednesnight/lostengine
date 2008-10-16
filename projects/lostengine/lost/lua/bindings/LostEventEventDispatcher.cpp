#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/event/EventDispatcher.h"

#include "lost/lua/bindings/LostEventEventDispatcher.h"

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

    void addEventListener(object dispatcher, const std::string& type, object func)
    {
      if(luabind::type(func) == LUA_TNIL) { throw std::runtime_error("can't register NIL lua callback function"); }
      EventDispatcher* disp = object_cast<EventDispatcher*>(dispatcher);
      disp->addEventListener(type, LuaHandlerFunction(func));
    }
  }
}

namespace lost
{
  namespace lua
  {
    void LostEventEventDispatcher(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("event")
        [
          class_<EventDispatcher>("EventDispatcher")
            .def(constructor<>())
            .def("addEventListener", (void(*)(object, const std::string&, object))&addEventListener)
        ]
      ];
    }  
  }
}
