#include "lost/lua/lua.h"
#include "lost/platform/Platform.h"

#include "lost/lua/bindings/LostPlatformPlatform.h"

using namespace luabind;
using namespace lost::platform;

namespace lost
{
  namespace lua
  {
    void LostPlatformPlatform(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("platform")
        [
          def("isWindows", &isWindows),
          def("isMac", &isMac),
          def("isLinux", &isLinux),
          def("isIPhone", &isIPhone)
        ]
      ];
    }
  }
}
