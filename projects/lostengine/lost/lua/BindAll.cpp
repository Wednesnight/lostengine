#include "lost/lua/lua.h"
#include "lost/lua/BindAll.h"
//#include "lost/lua/bindings/LostApplicationTimer.h"
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
//#include "lost/lua/bindings/LostApplicationTimerEvent.h"
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

namespace lost
{
  namespace lua
  {
    void bindAll(lua_State* state)
    {
      LostCommonDisplayAttributes(state);
      LostBitmapBitmap(state);
      LostCommonLog(state);
      LostResourceFile(state);
      LostResourceLoader(state);
      LostEventEvent(state);  
      LostEventEventDispatcher(state);
      LostApplicationApplication(state);
      LostApplicationKeyEvent(state);
//      LostApplicationTimerEvent(state);
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
//      LostApplicationTimer(state);
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
      
      luabind::bind_class_info(state);
    }
  }
}
