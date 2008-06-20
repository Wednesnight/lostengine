#include "luabindings.h"
#include "lost/lua/LuaBindings.h"

void applyLuaBindings(lost::lua::State& state)
{
  lost::lua::bindAll(state);
  bindEvent(state);
  bindMouseEvent(state);
  bindKeyEvent(state);
  bindEventDispatcher(state);
  bindApplication(state);
}



