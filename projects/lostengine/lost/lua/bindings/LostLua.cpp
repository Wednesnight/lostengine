#include "lost/lua/bindings/LostLua.h"
#include "lost/lua/lua.h"
#include "lost/lua/State.h"

#include <slub/slub.h>

using namespace lost::lua;
using namespace slub;

namespace lost
{
  namespace lua
  {
  
    int doResourceFile(const string& inRelativePath, lua_State* ls)
    {
      lua::State* s = lua::State::stateFromState(ls);
      return s->doResourceFile(inRelativePath);
    }  
    
    void LostLuaState(lua_State* state)
    {    
      package(state, "lost").package("lua")
        .function("doResourceFile", &doResourceFile)

        .clazz<State>("State")
          .method("doResourceFile", (int(State::*)(const string&)) &State::doResourceFile)
          .method("getScriptFilename", &State::getScriptFilename)
          .method("getScriptSource", &State::getScriptSource)
					.method("memUsage", &State::memUsage)
          .field("callstackSize", &State::callstackSize);
    }

    void LostLuaDebug(lua_State* state)
    {    
      package(state, "lost").package("lua")
        .clazz<lua_Debug>("lua_Debug")
          .field("event", &lua_Debug::event)
          .field("name", &lua_Debug::name)
          .field("namewhat", &lua_Debug::namewhat)
          .field("what", &lua_Debug::what)
          .field("source", &lua_Debug::source)
          .field("currentline", &lua_Debug::currentline)
          .field("nups", &lua_Debug::nups)
          .field("linedefined", &lua_Debug::linedefined)
          .field("lastlinedefined", &lua_Debug::lastlinedefined);
    }
    
    void LostLua(lua_State* state)
    {
      LostLuaState(state);
      LostLuaDebug(state);
    }

  }
}
