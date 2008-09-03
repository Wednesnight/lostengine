#include "lost/lua/bindings/LostResourceLoader.h"
#include "lost/resource/Loader.h"

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

LOST_LUA_EXPORT_BEGIN(LostResourceLoader)
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
LOST_LUA_EXPORT_END
