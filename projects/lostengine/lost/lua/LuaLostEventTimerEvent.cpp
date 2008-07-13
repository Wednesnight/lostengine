#include "lost/lua/Luabindings.h"
#include "lost/application/TimerEvent.h"
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace boost;
using namespace luabind;
using namespace lost::event;
using namespace lost::application;

// required for shared_ptr handling of Event objects 
namespace luabind {
  template<class T>
  T* get_pointer(boost::shared_ptr<T>& p) 
  {
    return p.get(); 
  }
  
  template<class T>
  boost::shared_ptr<const T>* 
  get_const_holder(boost::shared_ptr<T>*)
  {
    return 0;
  }
}

namespace lost
{
  namespace lua
  {
    void bindLostApplicationTimerEvent(lost::lua::State& state)
    {
      module(state, "lost")
      [
       namespace_("application")
       [
        class_<TimerEvent, shared_ptr<TimerEvent>, shared_ptr<Event> >("TimerEvent")
        .def(constructor<string>()) 
        .def_readwrite("passedSec", &TimerEvent::passedSec)
        ]
       ];
    }
  }
}