#include "luabindings.h"
#include "Event.h"

using namespace std;
using namespace luabind;

void bindEvent(lost::lua::State& state)
{
  module(state)
  [
    class_<Event, lost::shared_ptr<Event> >("Event")
      .def(constructor<string>())
      .def_readwrite("type", &Event::type)
  ];
}

// required for shared_ptr management of Events by Lua/luabind
namespace luabind {
    template<class Event>
    Event* get_pointer(lost::shared_ptr<Event>& p) 
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
