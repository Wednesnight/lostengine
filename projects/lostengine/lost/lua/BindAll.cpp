#include "lost/lua/lua.h"
#include "lost/lua/BindAll.h"
#include "lost/lua/bindings/LostApplicationApplicationEvent.h"
#include "lost/lua/bindings/LostApplicationTouchEvent.h"
#include "lost/lua/bindings/LostEventEvent.h"
#include "lost/lua/bindings/LostCommonLog.h"
#include "lost/lua/bindings/LostApplicationKeyEvent.h"
#include "lost/lua/bindings/LostGLGL.h"
#include "lost/lua/bindings/LostLGLLGL.h"
#include "lost/lua/bindings/LostGLUtils.h"
#include "lost/lua/bindings/LostCommonFpsMeter.h"
#include "lost/lua/bindings/LostApplicationResizeEvent.h"
#include "lost/lua/bindings/LostCommonColor.h"
#include "lost/lua/bindings/LostMathVec2.h"
#include "lost/lua/bindings/LostMathVec3.h"
#include "lost/lua/bindings/LostMathVec4.h"
#include "lost/lua/bindings/LostMathRect.h"
#include "lost/lua/bindings/LostModelObjParser.h"
#include "lost/lua/bindings/LostModelObjMaterial.h"
#include "lost/lua/bindings/LostModelMesh.h"
#include "lost/lua/bindings/LostGLDraw.h"
#include "lost/lua/bindings/LostCameraCamera.h"
#include "lost/lua/bindings/LostApplicationApplication.h"
#include "lost/lua/bindings/LostResourceLoader.h"
#include "lost/lua/bindings/LostResourceFile.h"
#include "lost/lua/bindings/LostModelObjRenderer.h"
#include "lost/lua/bindings/LostEventEventDispatcher.h"
#include "lost/lua/bindings/LostApplicationMouseEvent.h"
#include "lost/lua/bindings/LostLuaState.h"
#include "lost/lua/bindings/LostLSystemLSystem.h"
#include "lost/lua/bindings/LostApplicationAccelerometerEvent.h"
#include "lost/lua/bindings/LostCommonDisplayAttributes.h"
#include "lost/lua/bindings/LostBitmapBitmap.h"
#include "luabind/class_info.hpp"
#include "lost/lua/bindings/LostPlatformPlatform.h"
#include "lost/lua/bindings/LostGLContext.h"
#include "lost/lua/bindings/LostGLState.h"
#include "lost/lua/bindings/LostGLTexture.h"
#include "lost/lua/bindings/LostGLMesh.h"
#include "lost/lua/bindings/LostFontTrueTypeFont.h"
#include "lost/lua/bindings/LostFontModel.h"
#include "lost/lua/bindings/LostFontFreetypeLibrary.h"
#include "lost/lua/bindings/LostGLRenderBuffer.h"
#include "lost/lua/bindings/LostGLFrameBuffer.h"
#include "lost/lua/bindings/LostApplicationWindow.h"
#include "lost/lua/bindings/LostGLCanvas.h"
#include "lost/lua/bindings/LostApplicationInputEvent.h"


void openBaseLib(lua_State* state) { lua_pushcfunction(state, luaopen_base);lua_pushstring(state, "");lua_call(state, 1, 0); }
void openDebugLib(lua_State* state) { lua_pushcfunction(state, luaopen_debug);lua_pushstring(state, "debug");lua_call(state, 1, 0); }
void openIoLib(lua_State* state) { lua_pushcfunction(state, luaopen_io);lua_pushstring(state, "io");lua_call(state, 1, 0);}
void openMathLib(lua_State* state) { lua_pushcfunction(state, luaopen_math);lua_pushstring(state, "math");lua_call(state, 1, 0); }
void openPackageLib(lua_State* state) { lua_pushcfunction(state, luaopen_package);lua_pushstring(state, "package");lua_call(state, 1, 0); }
void openStringLib(lua_State* state) { lua_pushcfunction(state, luaopen_string);lua_pushstring(state, "string");lua_call(state, 1, 0); }
void openTableLib(lua_State* state) { lua_pushcfunction(state, luaopen_table);lua_pushstring(state, "table");lua_call(state, 1, 0); }
void openOsLib(lua_State* state) { lua_pushcfunction(state, luaopen_os);lua_pushstring(state, "os");lua_call(state, 1, 0); }

namespace lost
{
  namespace lua
  {
    void bindAll(lua_State* state)
    {
      openBaseLib(state);
      openDebugLib(state);
      openIoLib(state);
      openMathLib(state);
      openPackageLib(state);
      openStringLib(state);
      openTableLib(state);
      openOsLib(state);		
		
      LostCommonDisplayAttributes(state);
      LostBitmapBitmap(state);
      LostCommonLog(state);
      LostResourceFile(state);
      LostResourceLoader(state);
      LostEventEvent(state);  
      LostEventEventDispatcher(state);
      LostApplicationApplication(state);
      LostApplicationInputEvent(state);
      LostApplicationKeyEvent(state);
      LostApplicationResizeEvent(state);
      LostApplicationMouseEvent(state);
      LostApplicationApplicationEvent(state);
      LostLSystemLSystem(state);
      LostMathVec2(state);
      LostMathVec3(state);
      LostMathVec4(state);
      LostApplicationAccelerometerEvent(state);
      LostApplicationTouchEvent(state);
      LostCameraCamera(state);
      LostModelObjRenderer(state);
      LostModelMesh(state);
      LostModelObjMaterial(state);
      LostModelObjParser(state);
      LostLuaState(state);
      LostCommonColor(state);
      LostMathRect(state);
      LostCommonFpsMeter(state);
      LostGLGL(state);
      LostLGLLGL(state);
      LostGLDraw(state);
      LostGLUtils(state);
      LostPlatformPlatform(state);
      LostGLContext(state);
      LostGLState(state);
      LostGLTexture(state);
      LostGLMesh(state);
      LostFontTrueTypeFont(state);
      LostFontModel(state);
      LostFontFreetypeLibrary(state);
      LostGLRenderBuffer(state);
      LostGLFrameBuffer(state);
      LostApplicationWindow(state);
      LostGLCanvas(state);
      
      luabind::bind_class_info(state);
    }
  }
}
