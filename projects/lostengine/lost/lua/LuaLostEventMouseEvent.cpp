#include "lost/lua/Luabindings.h"
#include "lost/application/MouseEvent.h"
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
    void bindLostApplicationMouseEvent(lost::lua::State& state)
    {
      module(state, "lost")
      [
       namespace_("application")
       [
        class_<MouseEvent, shared_ptr<MouseEvent>, shared_ptr<Event> >("MouseEvent")
        .def(constructor<string>()) 
        .def_readwrite("pos", &MouseEvent::pos)
        .def_readwrite("button", &MouseEvent::button)
        .def_readwrite("pressed", &MouseEvent::pressed)
        ]
       ];
    }
  }
}