/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/lua/bindings/LostResource.h"
#include "lost/lua/lua.h"
#include "lost/resource/Loader.h"
#include "lost/resource/Bundle.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/resource/FilesystemRepository.h"
#include "lost/resource/ApplicationResourceRepository.h"
#include "lost/common/Data.h"

#include <slub/slub.h>

using namespace lost::resource;

namespace lost
{
  namespace lua
  {
    void LostResourceRepository(lua_State* state)
    {
      slub::package(state, "lost").package("resource")
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
      slub::package(state, "lost").package("resource")
        .clazz<Loader>("Loader")
          .method("load", &LostResourceLoader_load)
          .method("locate", &LostResourceLoader_locate)
          .method("addRepository", &Loader::addRepository)
//          .def("directory", &LostResourceLoader_directory, return_directory_iterator)
          .function("create", &Loader::create);
    }

    void LostResourceBundle(lua_State* state)
    {
      slub::package(state, "lost").package("resource")
      .clazz<Bundle>("Bundle")
      .extends<Loader>()
      //          .def("directory", &LostResourceLoader_directory, return_directory_iterator)
      .function("create", &Bundle::create);
    }

    void LostResourceFilesystemRepository(lua_State* state)
    {
      slub::package(state, "lost").package("resource")
        .clazz<FilesystemRepository>("FilesystemRepository")
          .extends<Repository>()
      .function("create", (RepositoryPtr(*)(const lost::fs::Path&)) &FilesystemRepository::create)
      .function("create", (RepositoryPtr(*)(const string&)) &FilesystemRepository::create);
    }
    
    void LostResourceApplicationResourceRepository(lua_State* state)
    {
      slub::package(state, "lost").package("resource")
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
