#include "lost/lua/Luabindings.h"
#include "lost/resource/Loader.h"

using namespace std;
using namespace luabind;
using namespace lost::resource;

// required for shared_ptr handling of Loader objects 
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

boost::shared_ptr<lost::resource::File> load(object inLoader, const std::string& inRelativePath)
{
  Loader* loader = object_cast<Loader*>(inLoader);
  return loader->load(inRelativePath);
}

void bindLostResourceLoader(lost::lua::State& state)
{
  module(state, "lost")
  [
    namespace_("resource")
    [
      class_<Loader, boost::shared_ptr<Loader> >("Loader")
        .def(constructor<>())
        .def("load", &load)
    ]
  ];
}

}
}