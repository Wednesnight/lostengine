#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/application/gl/Context.h"

#include "lost/lua/bindings/LostApplicationGLContext.h"

using namespace luabind;
using namespace lost::application::gl;

namespace lost
{
  namespace lua
  {
    void LostApplicationGLContext(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          namespace_("gl")
          [
            class_<Context, boost::shared_ptr<Context> >("Context")
              .def("makeCurrent", &Context::makeCurrent)
              .def("swapBuffers", &Context::swapBuffers)
              .def("pushState", &Context::pushState)
              .def("popState", &Context::popState)
          ]
        ]
      ];
    }
  }
}
