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

#include "lost/lua/lostlua.h"
#include "lost/lua/BindAll.h"

#include "lost/lua/bindings/LostApplication.h"
#ifndef ANDROID
  #include "lost/lua/bindings/LostAudio.h"
#endif
#include "lost/lua/bindings/LostBitmap.h"
#include "lost/lua/bindings/LostBox2D.h"
#include "lost/lua/bindings/LostCamera.h"
#include "lost/lua/bindings/LostCommon.h"
#include "lost/lua/bindings/LostEvent.h"
#include "lost/lua/bindings/LostFont.h"
#include "lost/lua/bindings/LostFs.h"
#include "lost/lua/bindings/LostGL.h"
#include "lost/lua/bindings/LostLua.h"
#include "lost/lua/bindings/LostMath.h"
#include "lost/lua/bindings/LostMesh.h"
#include "lost/lua/bindings/LostPlatform.h"
#include "lost/lua/bindings/LostProfiler.h"
#include "lost/lua/bindings/LostResource.h"
#include "lost/lua/bindings/LostRg.h"
#include "lost/lua/bindings/LostTime.h"
#include "lost/lua/bindings/ThirdpartyBox2D.h"


void BaseLib(lua_State* state) { lua_pushcfunction(state, luaopen_base);lua_pushstring(state, "");lua_call(state, 1, 0); }
void DebugLib(lua_State* state) { lua_pushcfunction(state, luaopen_debug);lua_pushstring(state, "debug");lua_call(state, 1, 0); }
void IoLib(lua_State* state) { lua_pushcfunction(state, luaopen_io);lua_pushstring(state, "io");lua_call(state, 1, 0);}
void MathLib(lua_State* state) { lua_pushcfunction(state, luaopen_math);lua_pushstring(state, "math");lua_call(state, 1, 0); }
void PackageLib(lua_State* state) { lua_pushcfunction(state, luaopen_package);lua_pushstring(state, "package");lua_call(state, 1, 0); }
void StringLib(lua_State* state) { lua_pushcfunction(state, luaopen_string);lua_pushstring(state, "string");lua_call(state, 1, 0); }
void TableLib(lua_State* state) { lua_pushcfunction(state, luaopen_table);lua_pushstring(state, "table");lua_call(state, 1, 0); }
void OsLib(lua_State* state) { lua_pushcfunction(state, luaopen_os);lua_pushstring(state, "os");lua_call(state, 1, 0); }

namespace lost
{
  namespace lua
  {
    void bindAll(lua_State* state)
    {
      BaseLib(state);
      DebugLib(state);
      IoLib(state);
      MathLib(state);
      PackageLib(state);
      StringLib(state);
      TableLib(state);
      OsLib(state);

      // other packages derive from Event package so register that first
      LostCommon(state);
      LostEvent(state);
      LostFs(state);
      LostResource(state);
      LostApplication(state);
#ifndef ANDROID
      LostAudio(state);
#endif
      LostBitmap(state);
      LostCamera(state);
      LostMesh(state); // must bind mesh before font since font already requires the binding for inheritance
      LostFont(state);
      LostGL(state);
      LostLua(state);
      LostMath(state);
      LostPlatform(state);
      LostProfiler(state);
      LostRg(state);
      LostTime(state);
      ThirdpartyBox2D(state);
      LostBox2D(state); // bind after all of box2d was bound
    }
  }
}
