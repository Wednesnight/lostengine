#include "lost/lua/bindings/LostEvent.h"
#include "lost/lua/lua.h"

#include "lost/event/Event.h"
#include "lost/event/EventDispatcher.h"

using namespace luabind;
using namespace lost::event;

namespace lost
{
  namespace lua
  {

//    lost::shared_ptr<Event> asSharedPtr(Event* event)
//    {
//      return event->shared_from_this();
//    }
    
    void LostEventEvent(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("event")
        [
          class_<Event>("Event")
            .def(constructor<std::string>()) 
            .def(constructor<const Event&>()) 
//            .def("asSharedPtr", &asSharedPtr)
            .def_readwrite("type", &Event::type)
        ]
      ];
    }

    struct LuaHandlerFunction
    {
      luabind::object func;
      
      LuaHandlerFunction(luabind::object inFunction) : func(inFunction) {}
      
      void operator()(EventPtr event)
      {
        luabind::call_function<void>(func, event);
      }
    };
    
    boost::signals::connection addEventListener(object dispatcher, const std::string& type, object func)
    {
      if(luabind::type(func) == LUA_TNIL) { throw std::runtime_error("can't register NIL lua callback function"); }
      EventDispatcher* disp = object_cast<EventDispatcher*>(dispatcher);
      return disp->addEventListener(type, LuaHandlerFunction(func));
    }

    struct LuaEventDispatcher : EventDispatcher, luabind::wrap_base
    {
      LuaEventDispatcher()
      : EventDispatcher()
      {}
      
      virtual void dispatchEvent(EventPtr event)
      { 
        call<void>("dispatchEvent", event); 
      }
      
      static void dispatchEventBase(EventDispatcher* dispatcher, EventPtr event)
      {
        return dispatcher->EventDispatcher::dispatchEvent(event);
      }
    };

    void LostEventEventDispatcher(lua_State* state)
    {
      module(state, "boost")
      [
        namespace_("signals")
        [
          class_<boost::signals::connection>("connection")
            .def(constructor<>())
        ]
      ];
      
      module(state, "lost")
      [
        namespace_("event")
        [
          class_<EventDispatcher, LuaEventDispatcher>("EventDispatcher")
            .def(constructor<>())
            .def("addEventListener", (boost::signals::connection(*)(object, const std::string&, object))&addEventListener)
            .def("removeEventListener", &EventDispatcher::removeEventListener)
            .def("dispatchEvent", &EventDispatcher::dispatchEvent, &LuaEventDispatcher::dispatchEventBase)
            .def("processEvents", &EventDispatcher::processEvents)
        ]
      ];
    }  

    void LostEvent(lua_State* state)
    {
      LostEventEvent(state);
      LostEventEventDispatcher(state);
    }

  }
}
