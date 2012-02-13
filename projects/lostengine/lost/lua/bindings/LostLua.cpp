/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/lua/bindings/LostLua.h"
#include "lost/lua/lostlua.h"
#include "lost/lua/State.h"

#include <slub/slub.h>

using namespace lost::lua;

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
      slub::package(state, "lost").package("lua")
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
      slub::package(state, "lost").package("lua")
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
