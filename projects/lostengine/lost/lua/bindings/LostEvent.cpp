#include "lost/lua/bindings/LostEvent.h"
#include "lost/lua/lua.h"

#include "lost/event/Event.h"
#include "lost/event/EventDispatcher.h"
#include "lost/event/Listener.h"
#include "lost/event/Connection.h"

#include <slub/slub.h>

using namespace lost::event;

namespace lost
{
  namespace lua
  {
    struct LuaHandlerFunction : lost::event::Listener
    {
      slub::reference func;
      
      LuaHandlerFunction(const slub::reference& inFunction) : func(inFunction) {}
      
      void call(const EventPtr& event) {
        func(event);
      }
    };
    
    lost::event::ConnectionPtr addEventListener(EventDispatcher* dispatcher, event::Type type,
                                                const slub::reference& func)
    {
      if(func.type() == LUA_TNIL) { throw std::runtime_error("can't register NIL lua callback function"); }
      return dispatcher->addEventListener(type, ListenerPtr(new LuaHandlerFunction(func)));
    }

    void LostEvent(lua_State* state)
    {
      slub::package event = slub::package(state, "lost").package("event");

      event.clazz<Event>("Event")
        .field("type", &Event::type)
        .field("bubbles", &Event::bubbles)
        .field("stopPropagation", &Event::stopPropagation)
        .field("stopDispatch", &Event::stopDispatch)
        .function("create", &Event::create);
        
      event.clazz<Connection>("Connection"),      

      event.clazz<EventDispatcher>("EventDispatcher")
        .constructor()
        .method("addEventListener", &addEventListener)
        .method("removeEventListener", &EventDispatcher::removeEventListener)
        .method("dispatchEvent", &EventDispatcher::dispatchEvent)
        .method("queueEvent", &EventDispatcher::queueEvent)
        .method("processEvents", &EventDispatcher::processEvents)
        .method("attachTo", &EventDispatcher::attachTo);
    }  
  }
}
