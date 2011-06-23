#include "lost/lua/bindings/LostEvent.h"
#include "lost/lua/lua.h"

#include "lost/event/Event.h"
#include "lost/event/EventDispatcher.h"
#include "lost/event/Listener.h"
#include "lost/event/Connection.h"

using namespace luabind;
using namespace lost::event;

namespace lost
{
  namespace lua
  {
    struct LuaHandlerFunction : lost::event::Listener
    {
      luabind::object func;
      
      LuaHandlerFunction(luabind::object inFunction) : func(inFunction) {}
      
      void call(const EventPtr& event)
      {
        luabind::call_function<void>(func, event);
      }
    };
    
    lost::event::ConnectionPtr addEventListener(object dispatcher, event::Type type, object func)
    {
      if(luabind::type(func) == LUA_TNIL) { throw std::runtime_error("can't register NIL lua callback function"); }
      EventDispatcher* disp = object_cast<EventDispatcher*>(dispatcher);
      return disp->addEventListener(type, ListenerPtr(new LuaHandlerFunction(func)));
    }

    void LostEvent(lua_State* state)
    {      
      module(state, "lost")
      [
        namespace_("event")
        [
          class_<Event>("Event")
            .def_readwrite("type", &Event::type)
            .scope
            [
              def("create", &Event::create)
            ],
        
          class_<Connection>("Connection"),
            

          class_<EventDispatcher>("EventDispatcher")
            .def(constructor<>())
            .def("addEventListener", &addEventListener)
            .def("removeEventListener", &EventDispatcher::removeEventListener)
            .def("dispatchEvent", &EventDispatcher::dispatchEvent)
            .def("queueEvent", &EventDispatcher::queueEvent)
            .def("processEvents", &EventDispatcher::processEvents)
            .def("attachTo", &EventDispatcher::attachTo)
        ]
      ];
    }  
  }
}
