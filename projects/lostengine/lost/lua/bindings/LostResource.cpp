#include "lost/lua/bindings/LostResource.h"
#include "lost/lua/lua.h"

#include "lost/resource/File.h"
#include "lost/resource/Loader.h"

using namespace luabind;
using namespace lost::resource;

namespace lost
{
  namespace lua
  {

    void LostResourceFile(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("resource")
        [
          class_<File, lost::shared_ptr<File> >("File")
            .def(constructor<>())    
            .def_readwrite("size", &File::size)
            .def_readwrite("location", &File::location)
            .def("str", &File::str)
        ]
      ];
    }

    FilePtr LostResourceLoader_load(object inLoader, const std::string& inRelativePath)
    {
      Loader* loader = object_cast<Loader*>(inLoader);
      return loader->load(inRelativePath);
    }

    void LostResourceLoader(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("resource")
        [
          class_<Loader, lost::shared_ptr<Loader> >("Loader")
            .def(constructor<>())
            .def("load", &LostResourceLoader_load)
        ]
      ];
    }

    void LostResource(lua_State* state)
    {
      LostResourceFile(state);
      LostResourceLoader(state);
    }

  }
}
