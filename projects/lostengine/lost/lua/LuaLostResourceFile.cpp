#include "lost/lua/Luabindings.h"
#include "lost/resource/File.h"
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace luabind;
using namespace lost::resource;

// required for shared_ptr handling of File objects 
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

void bindLostResourceFile(lost::lua::State& state)
{
  module(state, "lost")
  [
    namespace_("resource")
    [
      class_<File, boost::shared_ptr<File> >("File")
        .def(constructor<>())    
         .def_readwrite("size",     &File::size)
         .def_readwrite("location",     &File::location)
         .def("str", &File::str)
    ]
  ];
}

}
}