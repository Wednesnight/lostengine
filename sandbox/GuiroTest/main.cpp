#include "lost/common/Logger.h"
#include "lost/lua/LuaBindings.h"
#include "lost/gl/Draw.h"
#include "lost/application/Application.h"

using namespace std;
using namespace luabind;
using namespace lost::gl;
using namespace lost::common;
using namespace lost::application;

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

LOST_LUA_EXPORT_BEGIN(GuiroTestView)
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
LOST_LUA_EXPORT_END

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;

    // register test class
    GuiroTestView(*(app.interpreter));
    
    // run
    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}
