#include "lost/lua/Luabindings.h"
#include "lost/application/ApplicationEvent.h"
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
    void bindLostApplicationApplicationEvent(lost::lua::State& state)
    {
      module(state, "lost")
      [
       namespace_("application")
       [
        class_<ApplicationEvent, shared_ptr<ApplicationEvent>, shared_ptr<Event> >("ApplicationEvent")
        .def(constructor<string>()) 
        ]
       ];
    }
  }
}