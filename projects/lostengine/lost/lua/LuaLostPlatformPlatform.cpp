#include "lost/lua/LuaBindings.h"
#include "lost/platform/Platform.h"

using namespace luabind;
using namespace lost::platform;

LOST_LUA_EXPORT_BEGIN(LuaLostPlatformPlatform)
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
LOST_LUA_EXPORT_END
