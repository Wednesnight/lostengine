#include "lost/lua/bindings/LostPlatform.h"
#include "lost/lua/lua.h"

#include "lost/platform/Platform.h"
#include <boost/filesystem/path.hpp>

using namespace luabind;
using namespace lost::platform;

namespace lost
{
  namespace lua
  {

    void LostPlatFormFilesystem(lua_State* state)
    {
      module(state, "boost")
      [
        namespace_("filesystem")
        [
          class_<boost::filesystem::path>("path")
            .def(constructor<>())
            .def(constructor<const std::string&>())
        ]
      ];
    }
    
    void LostPlatformPlatform(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("platform")
        [
          def("isWindows", &isWindows),
          def("isMac", &isMac),
          def("isLinux", &isLinux),
          def("isIPhone", &isIPhone),
          def("currentTimeSeconds", &currentTimeSeconds),
          def("currentTimeMilliSeconds", &currentTimeMilliSeconds),
          def("buildResourcePath", &buildResourcePath),
          def("getResourcePath", &getResourcePath)
        ]
      ];
    }

    void LostPlatform(lua_State* state)
    {
      LostPlatFormFilesystem(state);
      LostPlatformPlatform(state);
    }

  }
}
