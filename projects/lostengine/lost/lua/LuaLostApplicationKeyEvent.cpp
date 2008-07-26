#include "lost/lua/Luabindings.h"
#include "lost/application/KeyEvent.h"
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace boost;
using namespace luabind;
using namespace lost::application;
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
    void bindLostApplicationKeyEvent(lost::lua::State& state)
    {
      module(state, "lost")
      [
       namespace_("application")
       [
        class_<KeyEvent, shared_ptr<KeyEvent>, shared_ptr<Event> >("KeyEvent")
        .def(constructor<string>()) 
        .def_readwrite("key", &KeyEvent::key)
        .def_readwrite("pressed", &KeyEvent::pressed)
        ]
       ];
      globals(state)["lost"]["application"]["KeyEvent"]["KEY_UP"] = KeyEvent::KEY_UP();
      globals(state)["lost"]["application"]["KeyEvent"]["KEY_DOWN"] = KeyEvent::KEY_DOWN();
    }
  }
}