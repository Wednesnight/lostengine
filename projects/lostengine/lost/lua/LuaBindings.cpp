#include "lost/lua/LuaBindings.h"
#include "lost/common/Logger.h"

namespace lost
{
  namespace lua
  {
    void bindAll(lost::lua::State& state)
    {
      LuaLostCommonDisplayAttributes(state);
      LuaLostBitmapBitmap(state);
      LuaLostBitmapBitmapLoader(state);
      LuaLostCommonLog(state);
      LuaLostResourceFile(state);
      LuaLostResourceLoader(state);
      LuaLostEventEvent(state);  
      LuaLostEventEventDispatcher(state);
      LuaLostApplicationApplication(state);
      LuaLostApplicationKeyEvent(state);
      LuaLostApplicationTimerEvent(state);
      LuaLostApplicationResizeEvent(state);
      LuaLostApplicationMouseEvent(state);
      LuaLostApplicationApplicationEvent(state);
      LuaLostLSystemLSystem(state);
      LuaLostMathVec3(state);
      LuaLostMathVec4(state);
      luabind::bind_class_info(state);
    }
  }
}
