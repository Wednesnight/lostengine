#ifndef LOST_LUA_STATE_H
#define LOST_LUA_STATE_H

#include "lost/lua/lua.h"
#include <string>
#include <map>
#include "lost/resource/File.h"
#include "lost/resource/DefaultLoader.h"
#include <boost/utility.hpp>

namespace lost
{
  namespace lua
  {

    struct State;
    typedef lost::shared_ptr<State> StatePtr;

    struct State : boost::noncopyable
    {
      State(lost::shared_ptr<resource::Loader> inLoader = lost::shared_ptr<resource::Loader>(new resource::DefaultLoader));
      ~State();

      std::string getScriptFilename(const std::string& scriptContent, const std::string& defaultName);
      std::string getScriptSource(lua_Debug& debug);
      static int errorHandler(lua_State* state);
      int handleError();

      // cast operator to original lua_State* value
      operator lua_State*() { return state; }

      // adds all standard libs to the state of the interpreter
      void openLibs()  { luaL_openlibs(state);  }
      // or fine grained functions if you only want to add single libs
      void openBaseLib() { lua_pushcfunction(state, luaopen_base);lua_pushstring(state, "");lua_call(state, 1, 0); }
      void openDebugLib() { lua_pushcfunction(state, luaopen_debug);lua_pushstring(state, "debug");lua_call(state, 1, 0); }
      void openIoLib() { lua_pushcfunction(state, luaopen_io);lua_pushstring(state, "io");lua_call(state, 1, 0);}
      void openMathLib() { lua_pushcfunction(state, luaopen_math);lua_pushstring(state, "math");lua_call(state, 1, 0); }
      void openPackageLib() { lua_pushcfunction(state, luaopen_package);lua_pushstring(state, "package");lua_call(state, 1, 0); }
      void openStringLib() { lua_pushcfunction(state, luaopen_string);lua_pushstring(state, "string");lua_call(state, 1, 0); }
      void openTableLib() { lua_pushcfunction(state, luaopen_table);lua_pushstring(state, "table");lua_call(state, 1, 0); }
      void openOsLib() { lua_pushcfunction(state, luaopen_os);lua_pushstring(state, "os");lua_call(state, 1, 0); }

      // convert naming convention "lost.lua..." to path "lost/lua/..."
      std::string pathFromNamespace(const std::string& inNamespace);

      // loads and executes a file from the resource directory
      int doResourceFile(const std::string& inRelativePath);

      // loads and executes a file
      int doFile(const std::string& inAbsolutePath);
      int doFile(const lost::shared_ptr<lost::resource::File>& inFile);

      // executes the given string as a lua program
      int doString(const std::string& inData);

      // returns memory usage of the lua state in kilobytes
      int memUsage() { return lua_getgccount(state); }

      lua_State* state;
      int callstackSize;
      std::map<std::string, std::string> fileHashes;
      lost::shared_ptr<resource::Loader> loader;
    };

  }
}


#endif
