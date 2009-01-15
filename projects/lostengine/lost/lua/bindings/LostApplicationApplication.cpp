#include "lost/lua/lua.h"
#include "lost/application/Application.h"
#include "lost/application/MainLoop.h"

#include "lost/lua/bindings/LostApplicationApplication.h"

using namespace boost;
using namespace luabind;
using namespace lost::application;
using namespace lost::event;

namespace lost
{
  namespace lua
  {
    struct LuaMainLoop
    {
      luabind::object func;
      
      LuaMainLoop(luabind::object inFunction) : func(inFunction) {}
      
      void operator()()
      {
        luabind::call_function<void>(func);
      }
    };

    void setMainLoop(object inApp, object func)
    {
      if(luabind::type(func) == LUA_TNIL) { throw std::runtime_error("can't register NIL lua main loop"); }
      Application* app = object_cast<Application*>(inApp);
      app->mainLoop.reset(new FunctorMainLoop(app->context, LuaMainLoop(func)));
    }
  }
}

namespace lost
{
  namespace lua
  {
    void LostApplicationApplication(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<Application, EventDispatcher>("Application")
           .def("quit", &Application::quit)
           .def("swapBuffers", &Application::swapBuffers)
           .def("setMainLoop", &setMainLoop)
           .def("processEvents", &Application::processEvents)
           .def_readonly("loader", &Application::loader)
           .def_readonly("displayAttributes", &Application::displayAttributes)
           .def_readonly("context", &Application::context)
           .def_readwrite("screen", &Application::screen)
        ]
      ];
    }
  }
}
