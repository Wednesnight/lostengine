#include "lost/lua/bindings/LostResource.h"
#include "lost/lua/lua.h"

#include "lost/resource/File.h"
#include "lost/resource/Loader.h"
#include "lost/resource/FilesystemRepository.h"
#include "lost/resource/ApplicationResourceRepository.h"

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
          class_<Loader, LoaderPtr >("Loader")
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
          class_<FilesystemRepository, RepositoryPtr>("FilesystemRepository")
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
          class_<ApplicationResourceRepository, RepositoryPtr>("ApplicationResourceRepository")
            .scope
            [
              def("create", &arepo_create)
            ]
        ]
      ];
    }
    
    void LostResource(lua_State* state)
    {
      LostResourceFile(state);
      LostFilesystemRepository(state);
      LostApplicationResourceRepository(state);
      LostResourceLoader(state);
    }

  }
}
