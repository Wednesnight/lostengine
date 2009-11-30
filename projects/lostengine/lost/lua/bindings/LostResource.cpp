#include "lost/lua/bindings/LostResource.h"
#include "lost/lua/lua.h"
#include <boost/filesystem.hpp>
#include "lost/resource/Loader.h"
#include "lost/resource/FilesystemRepository.h"
#include "lost/resource/ApplicationResourceRepository.h"

using namespace luabind;
using namespace lost::resource;

namespace lost
{
  namespace lua
  {


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
            .def(constructor<>())
            .def("load", &LostResourceLoader_load)
            .def("addRepository", &Loader::addRepository)
            .scope
            [
              def("create", &Loader::create)
            ]
        ]
      ];
    }

    RepositoryPtr fsrepo_create(const std::string& path)
    {
      return RepositoryPtr(new FilesystemRepository(path));
    }

    void LostFilesystemRepository(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("resource")
        [
          // FIXME: should be held by FilesystemRepositoryPtr, but luabind ...
          class_<FilesystemRepository>("FilesystemRepository")
            .scope
            [
              def("create", &fsrepo_create)
            ]
        ]
      ];
    }

    RepositoryPtr arepo_create()
    {
      return RepositoryPtr(new ApplicationResourceRepository());
    }
    
    void LostApplicationResourceRepository(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("resource")
        [
          // FIXME: should be held by ApplicationResourceRepositoryPtr, but luabind ...
          class_<ApplicationResourceRepository>("ApplicationResourceRepository")
            .scope
            [
              def("create", &arepo_create)
            ]
        ]
      ];
    }
    
    void LostResource(lua_State* state)
    {
      LostFilesystemRepository(state);
      LostApplicationResourceRepository(state);
      LostResourceLoader(state);
    }

  }
}
