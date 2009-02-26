#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/application/gl/State.h"

#include "lost/lua/bindings/LostApplicationGLState.h"

using namespace luabind;
using namespace lost::application::gl;

namespace lost
{
  namespace lua
  {
    void LostApplicationGLState(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          namespace_("gl")
          [
            class_<State, boost::shared_ptr<State> >("State")
              .def("create", (SharedState(State::*)(const SharedParam&))&State::create)
              .def("create", (SharedState(State::*)(const SharedParam&, const SharedParam&))&State::create)
              .def("create", (SharedState(State::*)(const SharedParam&, const SharedParam&, const SharedParam&))&State::create)
              .def("create", (SharedState(State::*)(const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&))&State::create)
              .def("create", (SharedState(State::*)(const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&))&State::create)
              .def("create", (SharedState(State::*)(const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&, const SharedParam&))&State::create)
          ]
        ]
      ];
    }
  }
}
