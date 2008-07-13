#ifndef LOST_LUA_LUABINDINGS_H
#define LOST_LUA_LUABINDINGS_H

#include "lost/lua/State.h"

namespace lost
{
namespace lua
{

void bindLostCommonDisplayAttributes(lost::lua::State& state);
void bindLostBitmapBitmap(lost::lua::State& state);
void bindLostBitmapBitmapLoader(lost::lua::State& state);
void bindLostCommonLog(lost::lua::State& state);
void bindLostResourceFile(lost::lua::State& state);
void bindLostResourceLoader(lost::lua::State& state);
void bindLostEventEvent(lost::lua::State& state);
void bindLostEventEventDispatcher(lost::lua::State& state);
void bindLostApplicationApplication(lost::lua::State& state);
void bindLostApplicationKeyEvent(lost::lua::State& state);
void bindLostApplicationTimerEvent(lost::lua::State& state);
void bindLostApplicationResizeEvent(lost::lua::State& state);
void bindLostApplicationMouseEvent(lost::lua::State& state);
void bindLostApplicationApplicationEvent(lost::lua::State& state);

static inline void bindAll(lost::lua::State& state)
{
  bindLostCommonDisplayAttributes(state);
  bindLostBitmapBitmap(state);
  bindLostBitmapBitmapLoader(state);
  bindLostCommonLog(state);
  bindLostResourceFile(state);
  bindLostResourceLoader(state);
  bindLostEventEvent(state);  
  bindLostEventEventDispatcher(state);
  bindLostApplicationApplication(state);
  bindLostApplicationKeyEvent(state);
  bindLostApplicationTimerEvent(state);
  bindLostApplicationResizeEvent(state);
  bindLostApplicationMouseEvent(state);
  bindLostApplicationApplicationEvent(state);
}


}
}

#endif