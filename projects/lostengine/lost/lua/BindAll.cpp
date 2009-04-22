#include "lost/lua/lua.h"
#include "lost/lua/BindAll.h"

#include "lost/lua/bindings/LostApplication.h"
#include "lost/lua/bindings/LostBitmap.h"
#include "lost/lua/bindings/LostCamera.h"
#include "lost/lua/bindings/LostCommon.h"
#include "lost/lua/bindings/LostEvent.h"
#include "lost/lua/bindings/LostFont.h"
#include "lost/lua/bindings/LostGL.h"

#include "lost/lua/bindings/LostLGLLGL.h"
#include "lost/lua/bindings/LostMathVec2.h"
#include "lost/lua/bindings/LostMathVec3.h"
#include "lost/lua/bindings/LostMathVec4.h"
#include "lost/lua/bindings/LostMathRect.h"
#include "lost/lua/bindings/LostResourceLoader.h"
#include "lost/lua/bindings/LostResourceFile.h"
#include "lost/lua/bindings/LostLuaState.h"
#include "lost/lua/bindings/LostLSystemLSystem.h"
#include "luabind/class_info.hpp"
#include "lost/lua/bindings/LostPlatformPlatform.h"
#include "lost/lua/bindings/LostMeshMesh.h"
#include "lost/lua/bindings/LostMeshLine.h"
#include "lost/lua/bindings/LostMeshQuad.h"
#include "lost/lua/bindings/LostMeshMaterial.h"


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

      LostResourceFile(state);
      LostResourceLoader(state);
      LostLSystemLSystem(state);
      LostMathVec2(state);
      LostMathVec3(state);
      LostMathVec4(state);
      LostLuaState(state);
      LostMathRect(state);
      LostLGLLGL(state);
      LostPlatformPlatform(state);
      LostMeshMaterial(state);
      LostMeshMesh(state);
      LostMeshLine(state);
      LostMeshQuad(state);
      
      luabind::bind_class_info(state);
    }
  }
}
