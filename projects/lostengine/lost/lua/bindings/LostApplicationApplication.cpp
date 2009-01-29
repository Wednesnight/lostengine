#include "lost/lua/lua.h"
#include "lost/application/Application.h"
#include "lost/application/RunLoop.h"

#include "lost/lua/bindings/LostApplicationApplication.h"

using namespace boost;
using namespace luabind;
using namespace lost::application;
using namespace lost::event;

namespace lost
{
  namespace lua
  {
    struct LuaRunLoop
    {
      luabind::object func;
      
      LuaRunLoop(luabind::object inFunction) : func(inFunction) {}
      
      void operator()()
      {
        luabind::call_function<void>(func);
      }
    };

    void setRunLoop(object inApp, object func)
    {
      if(luabind::type(func) == LUA_TNIL) { throw std::runtime_error("can't register NIL lua main loop"); }
      Application* app = object_cast<Application*>(inApp);
      app->runLoop.reset(new RunLoopFunctor(LuaRunLoop(func)));
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
//           .def("swapBuffers", &Application::swapBuffers)
           .def("setRunLoop", &setRunLoop)
           .def("processEvents", &Application::processEvents)
           .def("createWindow", &Application::createWindow)
//           .def_readonly("loader", &Application::loader)
//           .def_readonly("displayAttributes", &Application::displayAttributes)
//           .def_readonly("context", &Application::context)
//           .def_readwrite("screen", &Application::screen)
        ]
      ];
    }
  }
}
