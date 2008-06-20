#ifndef APPLICATION_H
#define APPLICATION_H

#include "Konfig.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/lua/State.h"
#include "luabindings.h"
#include "EventDispatcher.h"

struct Application : public EventDispatcher
{
  Konfig                          config;
  lost::common::DisplayAttributes display;
  lost::lua::State                interpreter;
  
  Application()
  {
      applyLuaBindings(interpreter);
      luabind::globals(interpreter)["display"] = &display;
      interpreter.doString("config = {}");
      interpreter.doResourceFile("application.lua");      
  }
  
  virtual ~Application()
  {
  }
};

#endif