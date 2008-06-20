#include "luabindings.h"
#include "MouseEvent.h"

using namespace std;
using namespace luabind;

// required for shared_ptr management of Events by Lua/luabind
namespace luabind {
    template<class Event>
    Event* get_pointer(boost::shared_ptr<Event>& p) 
    {
        return p.get(); 
    }

    template<class Event>
    boost::shared_ptr<const Event>* 
    get_const_holder(boost::shared_ptr<Event>*)
    {
        return 0;
    }
}

void bindMouseEvent(lost::lua::State& state)
{
  module(state)
  [
    class_<MouseEvent, Event, boost::shared_ptr<Event> >("MouseEvent")
      .def(constructor<string>())      
      .def_readwrite("x", &MouseEvent::x)
      .def_readwrite("y", &MouseEvent::y)
  ];
  LOST_LUA_CONST_START(MouseEvent, state)
    LOST_LUA_CONST(MouseEvent, MOUSE_UP);
    LOST_LUA_CONST(MouseEvent, MOUSE_DOWN);
    LOST_LUA_CONST(MouseEvent, MOUSE_MOVE);
  LOST_LUA_CONST_END
}
