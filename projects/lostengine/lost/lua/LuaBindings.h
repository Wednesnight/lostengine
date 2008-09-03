#ifndef LOST_LUA_LUABINDINGS_H
#define LOST_LUA_LUABINDINGS_H

// include bindinghelpers before any other luabind headers
#include "lost/lua/BindingHelpers.h"
#include "lost/lua/State.h"
#include "luabind/class_info.hpp"

namespace lost
{
  namespace lua
  {
    void LuaLostCommonDisplayAttributes(lost::lua::State& state);
    void LuaLostBitmapBitmap(lost::lua::State& state);
    void LuaLostBitmapBitmapLoader(lost::lua::State& state);
    void LuaLostCommonLog(lost::lua::State& state);
    void LuaLostResourceFile(lost::lua::State& state);
    void LuaLostResourceLoader(lost::lua::State& state);
    void LuaLostEventEvent(lost::lua::State& state);
    void LuaLostEventEventDispatcher(lost::lua::State& state);
    void LuaLostApplicationApplication(lost::lua::State& state);
    void LuaLostApplicationKeyEvent(lost::lua::State& state);
    void LuaLostApplicationTimerEvent(lost::lua::State& state);
    void LuaLostApplicationResizeEvent(lost::lua::State& state);
    void LuaLostApplicationMouseEvent(lost::lua::State& state);
    void LuaLostApplicationApplicationEvent(lost::lua::State& state);
    void LuaLostLSystemLSystem(lost::lua::State& state);
    void LuaLostMathVec2(lost::lua::State& state);
    void LuaLostMathVec3(lost::lua::State& state);
    void LuaLostMathVec4(lost::lua::State& state);
    void LuaLostApplicationAccelerometerEvent(lost::lua::State& state);
    void LuaLostApplicationTouchEvent(lost::lua::State& state);
    void LuaLostCameraCamera(lost::lua::State& state);
    void LuaLostModelObjRenderer(lost::lua::State& state);
    void LuaLostModelMesh(lost::lua::State& state);
    void LuaLostModelObjMaterial(lost::lua::State& state);
    void LuaLostModelObjParser(lost::lua::State& state);
    void LuaLostLuaState(lost::lua::State& state);
    void LuaLostCommonColor(lost::lua::State& state);
    void LuaLostMathRect(lost::lua::State& state);
    void LuaLostApplicationTimer(lost::lua::State& state);
    void LuaLostCommonFpsMeter(lost::lua::State& state);
    void LuaLostGLGL(lost::lua::State& state);
    void LuaLostGLDraw(lost::lua::State& state);
    void LuaLostGLUtils(lost::lua::State& state);

    void bindAll(lost::lua::State& state);
  }
}

#endif
