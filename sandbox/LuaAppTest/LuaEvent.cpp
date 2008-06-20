#include "luabindings.h"
#include "Event.h"
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace luabind;

void bindEvent(lost::lua::State& state)
{
  module(state)
  [
    class_<Event, boost::shared_ptr<Event> >("Event")
      .def(constructor<string>())
      .def_readwrite("type", &Event::type)
  ];
}

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
