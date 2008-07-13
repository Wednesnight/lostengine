#include "lost/lua/Luabindings.h"
#include "lost/event/Event.h"
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace boost;
using namespace luabind;
using namespace lost::event;

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
void bindLostEventEvent(lost::lua::State& state)
{
  module(state, "lost")
  [
    namespace_("event")
    [
      class_<Event, shared_ptr<Event> >("Event")
       .def(constructor<string>()) 
       .def_readwrite("type", &Event::type)
    ]
  ];
}
}
}