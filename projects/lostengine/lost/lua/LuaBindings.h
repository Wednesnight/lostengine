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

static inline void bindAll(lost::lua::State& state)
{
  bindLostCommonDisplayAttributes(state);
  bindLostBitmapBitmap(state);
  bindLostBitmapBitmapLoader(state);
  bindLostCommonLog(state);
}


}
}

#endif