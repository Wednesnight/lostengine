#include "lost/lua/bindings/LostLua.h"
#include "lost/lua/lua.h"

#include "lost/lua/State.h"

using namespace luabind;
using namespace lost::lua;

namespace lost
{
  namespace lua
  {

    int doResourceFile(const std::string& inRelativePath, lua_State* ls)
    {
        lua::State* s = lua::State::stateFromState(ls);
        return s->doResourceFile(inRelativePath);
    }

    void LostLuaState(lua_State* state)
    {
/*      module(state)
      [
      ];*/
    
      module(state, "lost")
      [
        namespace_("lua")
        [
            def("doResourceFile", &doResourceFile, raw(_2)),
          class_<State, lost::shared_ptr<State> >("State")
            .def(constructor<>())
            .def("doFile", (int(State::*)(const lost::shared_ptr<lost::resource::File>&)) &State::doFile)
            .def("doFile", (int(State::*)(const std::string&)) &State::doFile)
            .def("doResourceFile", (int(State::*)(const std::string&)) &State::doResourceFile)
            .def("getScriptFilename", &State::getScriptFilename)
            .def_readwrite("callstackSize", &State::callstackSize)
						.def("memUsage", &State::memUsage)
        ]
      ];
    }

    void LostLua(lua_State* state)
    {
      LostLuaState(state);
    }

  }
}