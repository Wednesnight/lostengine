#include "lost/lua/lua.h"
#include "lost/lua/BindAll.h"

#include "lost/lua/bindings/LostApplication.h"
#include "lost/lua/bindings/LostBitmap.h"
#include "lost/lua/bindings/LostCamera.h"
#include "lost/lua/bindings/LostCommon.h"
#include "lost/lua/bindings/LostEvent.h"
#include "lost/lua/bindings/LostFont.h"
#include "lost/lua/bindings/LostGL.h"
#include "lost/lua/bindings/LostLGL.h"
#include "lost/lua/bindings/LostLSystem.h"
#include "lost/lua/bindings/LostLua.h"
#include "lost/lua/bindings/LostMath.h"
#include "lost/lua/bindings/LostMesh.h"
#include "lost/lua/bindings/LostPlatform.h"
#include "lost/lua/bindings/LostResource.h"
#include "lost/lua/bindings/LostRg.h"

#include "luabind/class_info.hpp"


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
      LostEvent(state);
      LostApplication(state);
      LostBitmap(state);
      LostCamera(state);
      LostCommon(state);
      LostFont(state);
      LostGL(state);
      LostLGL(state);
      LostLSystem(state);
      LostLua(state);
      LostMath(state);
      LostPlatform(state);
      LostResource(state);
      LostMesh(state);
      LostRg(state);
      
      luabind::bind_class_info(state);
    }
  }
}
