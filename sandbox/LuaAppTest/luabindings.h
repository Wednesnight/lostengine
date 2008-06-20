#ifndef LUABINDINGS_H
#define LUABINDINGS_H

#include "lost/lua/State.h"

void applyLuaBindings(lost::lua::State& state);

void bindEvent(lost::lua::State& state);
void bindMouseEvent(lost::lua::State& state);
void bindKeyEvent(lost::lua::State& state);
void bindEventDispatcher(lost::lua::State& state);
void bindApplication(lost::lua::State& state);

#endif