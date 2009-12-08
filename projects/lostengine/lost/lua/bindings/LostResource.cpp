#include "lost/lua/bindings/LostResource.h"
#include "lost/lua/lua.h"
#include <boost/filesystem.hpp>
#include "lost/resource/Loader.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/resource/FilesystemRepository.h"
#include "lost/resource/ApplicationResourceRepository.h"

using namespace luabind;
using namespace lost::resource;

namespace lost
{
  namespace lua
  {
    void LostResourceRepository(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("resource")
        [
          class_<Repository>("Repository")
        ]
      ];
    }


    common::DataPtr LostResourceLoader_load(object inLoader, const std::string& inRelativePath)
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
          class_<Loader>("Loader")
            .def("load", &LostResourceLoader_load)
            .def("addRepository", &Loader::addRepository)
            .scope
            [
              def("create", &Loader::create)
            ]
        ]
      ];
    }

    void LostResourceDefaultLoader(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("resource")
        [
          class_<DefaultLoader, Loader>("DefaultLoader")
            .scope
            [
              def("create", &DefaultLoader::create)
            ]
        ]
      ];
    }

    void LostResourceFilesystemRepository(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("resource")
        [
          class_<FilesystemRepository, Repository>("FilesystemRepository")
            .scope
            [
              def("create", &FilesystemRepository::create)
            ]
        ]
      ];
    }
    
    void LostResourceApplicationResourceRepository(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("resource")
        [
          class_<ApplicationResourceRepository, Repository>("ApplicationResourceRepository")
            .scope
            [
              def("create", &ApplicationResourceRepository::create)
            ]
        ]
      ];
    }
    
    void LostResource(lua_State* state)
    {
      LostResourceRepository(state);
      LostResourceFilesystemRepository(state);
      LostResourceApplicationResourceRepository(state);
      LostResourceLoader(state);
      LostResourceDefaultLoader(state);
    }

  }
}
