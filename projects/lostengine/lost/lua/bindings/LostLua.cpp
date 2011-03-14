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
      module(state, "lost")
      [
        namespace_("lua")
        [
           def("doResourceFile", &doResourceFile),        
          class_<State>("State")
            .def("doResourceFile", (int(State::*)(const std::string&)) &State::doResourceFile)
            .def("getScriptFilename", &State::getScriptFilename)
            .def("getScriptSource", &State::getScriptSource)
            .def_readwrite("callstackSize", &State::callstackSize)
						.def("memUsage", &State::memUsage)
        ]
      ];
    }

    void LostLuaDebug(lua_State* state)
    {    
      module(state, "lost")
      [
        namespace_("lua")
        [
          class_<lua_Debug>("lua_Debug")
            .def_readonly("event", &lua_Debug::event)
            .def_readonly("name", &lua_Debug::name)
            .def_readonly("namewhat", &lua_Debug::namewhat)
            .def_readonly("what", &lua_Debug::what)
            .def_readonly("source", &lua_Debug::source)
            .def_readonly("currentline", &lua_Debug::currentline)
            .def_readonly("nups", &lua_Debug::nups)
            .def_readonly("linedefined", &lua_Debug::linedefined)
            .def_readonly("lastlinedefined", &lua_Debug::lastlinedefined)
        ]
      ];
    }
    
    void LostLua(lua_State* state)
    {
      LostLuaState(state);
      LostLuaDebug(state);
    }

  }
}
