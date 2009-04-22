#include "lost/lua/lua.h"
#include "lost/resource/Loader.h"

#include "lost/lua/bindings/LostResourceLoader.h"

using namespace luabind;
using namespace lost::resource;

namespace lost
{
  namespace lua
  {
    lost::shared_ptr<lost::resource::File> load(object inLoader, const std::string& inRelativePath)
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
    void LostResourceLoader(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("resource")
        [
          class_<Loader, lost::shared_ptr<Loader> >("Loader")
            .def(constructor<>())
            .def("load", &load)
        ]
      ];
    }
  }
}
