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

#include "lost/lua/bindings/LostPlatform.h"
#include "lost/lua/lostlua.h"

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
