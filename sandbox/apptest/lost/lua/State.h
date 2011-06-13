#ifndef LOST_LUA_STATE_H
#define LOST_LUA_STATE_H

#include "EASTL/string.h"
#include "EASTL/map.h"
#include "lost/common/Data.h"
#include "lost/resource/DefaultLoader.h"
//#include <boost/utility.hpp>
#include "lost/lua/forward.h"
#include "lost/lua/lua.h"

namespace lost
{
  namespace lua
  {


    struct State //: boost::noncopyable
    {
      State(resource::LoaderPtr inLoader = resource::LoaderPtr(new resource::DefaultLoader));
      ~State();

      eastl::string getScriptFilename(const eastl::string& scriptContent, const eastl::string& defaultName);
      eastl::string getScriptSource(lua_Debug& debug);
      static State* stateFromState(lua_State* state);
      static int errorHandler(lua_State* state);
      int handleError();

      // cast operator to original lua_State* value
      operator lua_State*() { return state; }

      // adds all standard libs to the state of the interpreter
      void openLibs();
      // or fine grained functions if you only want to add single libs
      void openBaseLib();
      void openDebugLib();
      void openIoLib();
      void openMathLib();
      void openPackageLib();
      void openStringLib();
      void openTableLib();
      void openOsLib();

      void addScriptPathEntry(const eastl::string& inScript, const eastl::string& inAbsolutePath);

      // convert naming convention "lost.lua..." to path "lost/lua/..."
      eastl::string pathFromNamespace(const eastl::string& inNamespace);

      // loads and executes a file from the resource directory
      int doResourceFile(const eastl::string& inRelativePath);

      // loads and executes a file
      int doFile(const eastl::string& inAbsolutePath);

      // executes the given string as a lua program
      int doString(const eastl::string& inData);

      // returns memory usage of the lua state in kilobytes
      int memUsage() { return lua_getgccount(state); }

      eastl::string getFilenameFuncnameLine(); // formatted for logging

      lua_State* state;
      int callstackSize;
      eastl::map<eastl::string, eastl::string> fileHashes;
      resource::LoaderPtr loader;
      luabind::object globals;
    };

  }
}


#endif
