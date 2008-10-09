#include <boost/shared_ptr.hpp>
#include "lost/lua/State.h"
#include "lost/resource/Loader.h"

#include "lost/lua/bindings/LostResourceLoader.h"

using namespace luabind;
using namespace lost::resource;

namespace lost
{
  namespace lua
  {
    boost::shared_ptr<lost::resource::File> load(object inLoader, const std::string& inRelativePath)
    {
      Loader* loader = object_cast<Loader*>(inLoader);
      return loader->load(inRelativePath);
    }
  }
}

namespace lost
{
  namespace lua
  {
    void LostResourceLoader(lost::lua::State& state)
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
