/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/lua/bindings/LostEvent.h"
#include "lost/lua/lostlua.h"

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
        slub::call<void, const EventPtr&>(func, event);
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
