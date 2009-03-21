#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/gl/Context.h"

#include "lost/lua/bindings/LostGLContext.h"

using namespace luabind;
using namespace lost::gl;

namespace lost
{
  namespace lua
  {
    void LostGLContext(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("gl")
        [
          class_<Context, boost::shared_ptr<Context> >("Context")
            .def("makeCurrent", &Context::makeCurrent)
            .def("swapBuffers", &Context::swapBuffers)
            .def("pushState", &Context::pushState)
            .def("popState", &Context::popState)
            .def("vsync", &Context::vsync)
            .def("multithreaded", &Context::multithreaded)
        ]
      ];
    }
  }
}
