#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/lua/State.h"
#include "lost/resource/File.h"

#include "lost/lua/bindings/LostLuaState.h"

using namespace luabind;
using namespace lost::lua;

namespace lost
{
  namespace lua
  {
    void LostLuaState(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("lua")
        [
          class_<State, boost::shared_ptr<State> >("State")
            .def(constructor<>())
            .def("doFile", (int(State::*)(const boost::shared_ptr<lost::resource::File>&)) &State::doFile)
            .def("doFile", (int(State::*)(const std::string&)) &State::doFile)
            .def("doResourceFile", (int(State::*)(const std::string&)) &State::doResourceFile)
            .def_readwrite("callstackSize", &State::callstackSize)
        ]
      ];
    }
  }
}
