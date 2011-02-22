#include "lost/lua/bindings/LostResource.h"
#include "lost/lua/lua.h"
#include "lost/resource/Loader.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/resource/FilesystemRepository.h"
#include "lost/resource/ApplicationResourceRepository.h"
#include "lost/resource/Writer.h"
#include "lost/common/Data.h"
#include "lost/lua/bindings/directory_iterator_policy.h"

#include <boost/filesystem.hpp>

using namespace boost::filesystem;
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

    std::string LostResourceLoader_locate(object inLoader, const std::string& inRelativePath)
    {
      Loader* loader = object_cast<Loader*>(inLoader);
      return loader->locate(inRelativePath);
    }

    path LostResourceLoader_directory(object inLoader, const std::string& inRelativePath)
    {
      Loader* loader = object_cast<Loader*>(inLoader);
      return path(loader->locate(inRelativePath));
    }

    void LostResourceLoader(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("resource")
        [
          class_<Loader>("Loader")
            .def("load", &LostResourceLoader_load)
            .def("locate", &LostResourceLoader_locate)
            .def("addRepository", &Loader::addRepository)
            .def("directory", &LostResourceLoader_directory, return_directory_iterator)
        ]
      ];
    }

    void LostResourceWriter_write(object inWriter, const std::string& inPath, const common::DataPtr& inData)
    {
      Writer* writer = object_cast<Writer*>(inWriter);
      return writer->write(inPath, inData);
    }
    
    void LostResourceWriter(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("resource")
        [
          class_<Writer>("Writer")
            .def("write", &LostResourceWriter_write)
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
      LostResourceWriter(state);
    }

  }
}
