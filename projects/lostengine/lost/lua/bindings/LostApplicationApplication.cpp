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
      luabind::object runLoop;
      
      LuaRunLoop(luabind::object inRunLoop) : runLoop(inRunLoop) {}
      
      void operator()(const boost::shared_ptr<Application>& sender)
      {
        luabind::call_function<void>(runLoop, sender);
      }
    };

    void setRunLoop(object inApplication, object inRunLoop)
    {
      if(luabind::type(inRunLoop) != LUA_TFUNCTION) throw std::runtime_error("can't set non-function as runloop");
      Application* application = object_cast<Application*>(inApplication);
      application->setRunLoop(LuaRunLoop(inRunLoop));
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
          class_<Application, EventDispatcher, boost::shared_ptr<Application> >("Application")
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
