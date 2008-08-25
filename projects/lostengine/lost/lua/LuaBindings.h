#ifndef LOST_LUA_LUABINDINGS_H
#define LOST_LUA_LUABINDINGS_H

#define LOST_LUA_EXPORT_BEGIN(s) namespace lost\
                                 {\
                                   namespace lua\
                                   {\
                                     void s(lost::lua::State& state)
#define LOST_LUA_EXPORT_END        }\
                                 }

// this block is essential for correct luabind handling of boost::shared_ptr
#include <boost/shared_ptr.hpp>
namespace luabind
{
  template<class T>
  T* get_pointer(boost::shared_ptr<T>& p)
  {
    return p.get();
  }
  template<class T>
  boost::shared_ptr<const T>*
  get_const_holder(boost::shared_ptr<T>*)
  {
    return 0;
  }
}

#include <vector>
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
    void LuaLostMathVec3(lost::lua::State& state);
    void LuaLostMathVec4(lost::lua::State& state);
    void LuaLostApplicationAccelerometerEvent(lost::lua::State& state);
    void LuaLostApplicationTouchEvent(lost::lua::State& state);
    void LuaLostCameraCamera(lost::lua::State& state);
    void LuaLostModelObjRenderer(lost::lua::State& state);
    void LuaLostModelMesh(lost::lua::State& state);
    void LuaLostModelObjMaterial(lost::lua::State& state);
    void LuaLostModelObjParser(lost::lua::State& state);

    void bindAll(lost::lua::State& state);
  }
}

#endif
