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

#include "lost/lua/lua.h"
#include "lost/lua/State.h"
#include "lost/common/Logger.h"
#include <stdexcept>
#include <iostream>

#pragma warning(disable:4996) // no deprecated warnings for hashlib++
#include <md5.h>

static lost::map<lua_State*, lost::lua::State*> stateMap;

namespace lost
{
  namespace lua
  {

    struct LoadException : std::runtime_error
    {
      LoadException(const string& v) : runtime_error(v.c_str()){};
    };

    void State::openLibs()  { luaL_openlibs(state);  }
    void State::openBaseLib() { lua_pushcfunction(state, luaopen_base);lua_pushstring(state, "");lua_call(state, 1, 0); }
    void State::openDebugLib() { lua_pushcfunction(state, luaopen_debug);lua_pushstring(state, "debug");lua_call(state, 1, 0); }
    void State::openIoLib() { lua_pushcfunction(state, luaopen_io);lua_pushstring(state, "io");lua_call(state, 1, 0);}
    void State::openMathLib() { lua_pushcfunction(state, luaopen_math);lua_pushstring(state, "math");lua_call(state, 1, 0); }
    void State::openPackageLib() { lua_pushcfunction(state, luaopen_package);lua_pushstring(state, "package");lua_call(state, 1, 0); }
    void State::openStringLib() { lua_pushcfunction(state, luaopen_string);lua_pushstring(state, "string");lua_call(state, 1, 0); }
    void State::openTableLib() { lua_pushcfunction(state, luaopen_table);lua_pushstring(state, "table");lua_call(state, 1, 0); }
    void State::openOsLib() { lua_pushcfunction(state, luaopen_os);lua_pushstring(state, "os");lua_call(state, 1, 0); }

    void translateLoadException(lua_State* L, lost::lua::LoadException const& ex)
    {
      lua_pushstring(L, ex.what());
    }
    
    int catch_lua_error(lua_State* L) {
      if (lua_isstring(L, 1) && stateMap.find(L) != stateMap.end()) {
        return stateMap[L]->handleError();
      }
      return 0;
    }
    
    State::State(lost::shared_ptr<resource::Loader> inLoader)
    : callstackSize(10), 
      loader(inLoader),
      state(luaL_newstate()),
      globals(slub::globals(state))
    {
      stateMap[state] = this;
      slub::set_error_handler(catch_lua_error);
    }
    
    State::~State()
    {
      // set loader to nil because it's usually contained in state
      this->loader.reset();
      lua_gc(state, LUA_GCCOLLECT, 0);

      // close state
      lua_close(state);

      map<lua_State*, lost::lua::State*>::iterator pos;
      pos = stateMap.find(state);
      if(pos != stateMap.end())
        stateMap.erase(pos);
//      DOUT("stateMap size: "<<stateMap.size());
    }
    
    string State::getScriptFilename(const string& scriptContent, const string& defaultName)
    {
      string file(md5(scriptContent.c_str()).c_str());
      if (fileHashes.find(file) != fileHashes.end()) return fileHashes[file];
        else return defaultName;
    }
    
    string State::getScriptSource(lua_Debug& debug)
    {
      return getScriptFilename(debug.source, debug.short_src);
    }

    State* State::stateFromState(lua_State* state)
    {
        return stateMap[state];
    }

    int State::errorHandler(lua_State* state)
    {
      int result = 0;
      State* s = stateMap[state];
      result = s->handleError();
      return result;
    }
    
    int State::handleError()
    {
      lua_Debug debug;
      common::StringStream msg;
      msg << "Trace:";
      int depth = 1;
      for (unsigned int idx = 2; (lua_getstack(state, idx, &debug) == 1)
           && depth <= callstackSize; ++idx)
      {
        string name = "";
        string scriptpath = "";
        int32_t currentLine = -1;

        lua_getinfo(state, "Sln", &debug);

        // line and path
        if (debug.currentline >= 0) 
        {
          currentLine = debug.currentline;
          scriptpath = getScriptSource(debug);
        }
        // name
        if(debug.name != 0)
        {
          name = debug.name; 
        }
        else
        {
          name = "???";
        }

        // filter builtins that produce only noise in stack trace
        // these have to be adjusted should they ever change
        // require
        if((currentLine == -1) && (scriptpath.length() == 0) && (name=="require")) continue;
        // ModuleLoader
        if(scriptpath == "ModuleLoader.cpp") continue;
        if((currentLine == -1) && (scriptpath.length() == 0) && (name=="doResourceFile")) continue;

        // finally, if it wasn't filtered, build the stack frame message
        msg << "\n  ";
        if(currentLine >= 0) {
          msg << scriptpath << ":" << currentLine << ":";
        }
        else {
          msg << "::";
        }
        msg << name;
        ++depth;
      }
      
      EOUT(lua_tostring(state, -1) << "\n" << msg.str());

      return 1;
    }

    string State::pathFromNamespace(const string& inNamespace)
    {
      // convert naming convention "lost.lua..." to path "lost/lua/..."
      string filename(inNamespace);
      size_t pos;
      while ((pos = filename.find(".")) != filename.npos && pos != filename.find(".lua"))
      {
        filename.replace(pos, 1, "/");
      }
      return filename;
    }
    
    // loads and executes a file from the resource directory
    int State::doResourceFile(const string& inRelativePath)
    {
      return doFile(pathFromNamespace(inRelativePath));
    }
    
    void State::addScriptPathEntry(const string& inScript, const string& inAbsolutePath)
    {
      fileHashes[md5(inScript.c_str()).c_str()] = inAbsolutePath;      
    }
    
    // loads and executes a file
    int State::doFile(const string& inAbsolutePath)
    {
      std::ostringstream os;
      shared_ptr<common::Data> rawData = loader->load(inAbsolutePath);
      string data = rawData->str();
      addScriptPathEntry(data, inAbsolutePath);
      return doString(data);
    }

    // executes the given string as a lua program
    int State::doString(const string& inData)
    {
      // execute the loaded file and handle errors
      int err = luaL_loadstring(state, inData.c_str());
      if(!err) {
        err = slub::pcall(state, 0, LUA_MULTRET, catch_lua_error);
      }

      if(err) {
        string errcode;
        switch(err)
        {
          case LUA_YIELD:errcode="LUA_YIELD";break;
          case LUA_ERRRUN:errcode="LUA_ERRRUN";break;
          case LUA_ERRSYNTAX:errcode="LUA_ERRSYNTAX";break;
          case LUA_ERRMEM:errcode="LUA_ERRMEM";break;
          case LUA_ERRERR:errcode="LUA_ERRERR";break;
          default:errcode="??";break;
        }
        string errstring;
        const char* errstringc = lua_tostring(state, -1);
        lua_pop(state,1);
        if (errstringc != NULL)
        {
          errstring = errstringc;
          throw LoadException(getScriptFilename(inData, inData) +" ("+ errcode   +"): "+ errstring);
        }
        else
        {
          throw LoadException(getScriptFilename(inData, inData) +" ("+ errcode  +")");
        }
      }

      return err;
    }

    string State::getFilenameFuncnameLine()
    {
      lua_Debug debug;
      string result = "(Lua)";
      if(lua_getstack(state, 1, &debug)==1)
      {
        lua_getinfo(state, "Sln", &debug);
        string filename = getScriptFilename(debug.source, "<unknown>");
        string funcname = debug.name ? debug.name : "<unknown>";
        common::StringStream os;
        os <<"("<< filename << " " << funcname << " " << debug.currentline<<")";
        result = os.str();
      }
      return result;
    }
    
  }
}
