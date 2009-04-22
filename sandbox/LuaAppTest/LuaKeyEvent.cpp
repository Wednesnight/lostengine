#include "luabindings.h"
#include "KeyEvent.h"

using namespace std;
using namespace luabind;

// required for shared_ptr management of Events by Lua/luabind
namespace luabind {
    template<class Event>
    KeyEvent* get_pointer(lost::shared_ptr<KeyEvent>& p) 
    {
        return p.get(); 
    }

    template<class Event>
    lost::shared_ptr<const Event>* 
    get_const_holder(lost::shared_ptr<Event>*)
    {
        return 0;
    }
}

void bindKeyEvent(lost::lua::State& state)
{
  module(state)
  [
    class_<KeyEvent, Event, lost::shared_ptr<Event> >("KeyEvent")
      .def(constructor<string>())      
      .def_readwrite("keycode", &KeyEvent::keycode)
  ];
  LOST_LUA_CONST_START(KeyEvent, state)
    LOST_LUA_CONST(KeyEvent, KEY_DOWN);
    LOST_LUA_CONST(KeyEvent, KEY_UP);
  LOST_LUA_CONST_END  
}
