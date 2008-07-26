#include "lost/lua/Luabindings.h"
#include "lost/application/ResizeEvent.h"
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
    void bindLostApplicationResizeEvent(lost::lua::State& state)
    {
      module(state, "lost")
      [
       namespace_("application")
       [
        class_<ResizeEvent, shared_ptr<ResizeEvent>, shared_ptr<Event> >("ResizeEvent")
        .def(constructor<string>()) 
        .def_readwrite("width", &ResizeEvent::width)
        .def_readwrite("height", &ResizeEvent::height)
        ]
       ];
      
      globals(state)["lost"]["application"]["ResizeEvent"]["MAIN_WINDOW_RESIZE"] = ResizeEvent::MAIN_WINDOW_RESIZE();
    }
  }
}