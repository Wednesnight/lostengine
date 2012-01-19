#include "lost/lua/bindings/LostPlatform.h"
#include "lost/lua/lua.h"

#include "lost/platform/Platform.h"

using namespace slub;
using namespace lost::platform;

namespace lost
{
  namespace lua
  {

    void LostPlatformPlatform(lua_State* state)
    {
      package p = package(state, "lost").package("platform");
      p.function("isWindows", &isWindows);
      p.function("isMac", &isMac);
      p.function("isLinux", &isLinux);
      p.function("isIPhone", &isIPhone);
      p.function("currentTimeSeconds", &currentTimeSeconds);
      p.function("currentTimeMilliSeconds", &currentTimeMilliSeconds);
      p.function("getResourcePath", &getResourcePath);
      p.function("getUserDataPath", &getUserDataPath);
      p.function("getClipboardString", &getClipboardString);
      p.function("setClipboardString", &setClipboardString);
    }

    void LostPlatform(lua_State* state)
    {
      LostPlatformPlatform(state);
    }

  }
}
