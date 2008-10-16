#ifndef GUIROTEST_H
#define GUIROTEST_H

#include "lost/common/Logger.h"
#include "lost/gl/Draw.h"
#include "lost/event/EventDispatcher.h"
#include "lost/lua/State.h"

using namespace std;
using namespace luabind;
using namespace lost::gl;
using namespace lost::event;

struct View
{
  View()
  {
    DOUT("View::View()");
  }
  
  virtual ~View()
  {
    DOUT("View::~View()");
  }
  
  virtual void internalRender()
  {
    render();
  }
  
  virtual void render()
  {
    DOUT("View::render()");
  }
};

struct ViewLuaWrapper : View, luabind::wrap_base
{
  ViewLuaWrapper()
  : View()
  {
  }
  
  virtual void render()
  {
    call<void>("render");
  }
  
  static void renderBase(View* base)
  {
    return base->View::render();
  }
  
  virtual void internalRender()
  {
    call<void>("internalRender");
  }
  
  static void internalRenderBase(View* base)
  {
    return base->View::internalRender();
  }
};

namespace lost
{
  namespace lua
  {
    void GuiroTestView(lost::lua::State& state)
    {
      module(state, "lost")
      [
       namespace_("guiro")
       [
        class_<View, ViewLuaWrapper>("View")
        .def(constructor<>())
        .def("render", &View::render, &ViewLuaWrapper::renderBase)
        .def("internalRender", &View::internalRender, &ViewLuaWrapper::internalRenderBase)
        ]
       ];
    }
  }
}

#endif
