#include "lost/lua/bindings/LostResource.h"
#include "lost/lua/lua.h"
#include "lost/resource/Loader.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/resource/FilesystemRepository.h"
#include "lost/resource/ApplicationResourceRepository.h"
#include "lost/common/Data.h"

#include <slub/slub.h>

using namespace lost::resource;
using namespace slub;

namespace lost
{
  namespace lua
  {
    void LostResourceRepository(lua_State* state)
    {
      package(state, "lost").package("resource")
        .clazz<Repository>("Repository");
    }

    common::DataPtr LostResourceLoader_load(Loader* loader, const string& inRelativePath)
    {
      return loader->load(inRelativePath);
    }

    string LostResourceLoader_locate(Loader* loader, const string& inRelativePath)
    {
      return loader->locate(inRelativePath);
    }

/*
    path LostResourceLoader_directory(Loader* loader, const string& inRelativePath)
    {
      return path(loader->locate(inRelativePath));
    }
*/

    void LostResourceLoader(lua_State* state)
    {
      package(state, "lost").package("resource")
        .clazz<Loader>("Loader")
          .method("load", &LostResourceLoader_load)
          .method("locate", &LostResourceLoader_locate)
          .method("addRepository", &Loader::addRepository)
//          .def("directory", &LostResourceLoader_directory, return_directory_iterator)
          .function("create", &Loader::create);
    }
    
    void LostResourceFilesystemRepository(lua_State* state)
    {
      package(state, "lost").package("resource")
        .clazz<FilesystemRepository>("FilesystemRepository")
          .extends<Repository>()
          .function("create", &FilesystemRepository::create);
    }
    
    void LostResourceApplicationResourceRepository(lua_State* state)
    {
      package(state, "lost").package("resource")
        .clazz<ApplicationResourceRepository>("ApplicationResourceRepository")
          .extends<Repository>()
          .function("create", &ApplicationResourceRepository::create);
    }
    
    void LostResource(lua_State* state)
    {
      LostResourceRepository(state);
      LostResourceFilesystemRepository(state);
      LostResourceApplicationResourceRepository(state);
      LostResourceLoader(state);
    }

  }
}
