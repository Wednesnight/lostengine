#include "lost/lua/State.h"
#include "lost/lua/ErrorHandler.h"
#include "lost/platform/Platform.h"
#include "lost/common/Logger.h"
#include <stdexcept>
#include <boost/shared_ptr.hpp>

// no deprecated warnings for hashlib++
#pragma warning(disable:4996)
#include <hashlibpp.h>

using namespace boost;
using namespace std;

namespace lost
{
  namespace lua
  {
    
    State::State(boost::shared_ptr<resource::Loader> inLoader)
    : callstackSize(10), loader(inLoader)
    {
      state = luaL_newstate();
      luabind::open(state);      
      // set our own error callback
      luabind::set_pcall_callback(lost::lua::errorHandler);
    }
    
    State::~State()
    {
      lua_close(state);
    }
    
    std::string State::getScriptFilename(const std::string& scriptContent, const std::string& defaultName)
    {
      md5wrapper  md5;
      std::string file(md5.getHashFromString(scriptContent));
      if (fileHashes.find(file) != fileHashes.end()) return fileHashes[file];
        else return defaultName;
    }
    
    std::string State::getScriptSource(lua_Debug& debug)
    {
      std::string result("source: ");
      result.append(getScriptFilename(debug.source, debug.short_src));
      return result;
    }
    
    int State::handleError()
    {
			EOUT("////////////////// ERROR");
      lua_Debug debug;
      lua_getstack(state, 1, &debug);
      lua_getinfo(state, "Sln", &debug);
      
      // get call stack
//      std::string offset;
      for (unsigned int idx = callstackSize; idx > 0; --idx)
      {
        std::stringstream msg;
        if (lua_getstack(state, idx, &debug) == 1)
        {
          lua_getinfo(state, "Sln", &debug);
          msg << "-> " << debug.what;
          if (debug.namewhat != "") msg << " " << debug.namewhat;
            else msg << " unknown";
          if (debug.name != 0) msg << " " << debug.name;
          if (debug.currentline >= 0) msg  << " (" << getScriptSource(debug) << ", line " << debug.currentline << ")";
          
          // print out info
          EOUT(msg.str());

          // format next message
//          offset.append("  ");
        }
      }
      
      // lua error message
      std::string error;
      const char* errorc = lua_tostring(state, -1);
      if (errorc != NULL) error = errorc;
      lua_pop(state, 1);
      EOUT(error);

      return 1;
    }

    std::string State::pathFromNamespace(const std::string& inNamespace)
    {
      // convert naming convention "lost.lua..." to path "lost/lua/..."
      std::string filename(inNamespace);
      size_t pos;
      while ((pos = filename.find(".")) != filename.npos && pos != filename.find(".lua"))
      {
        filename.replace(pos, 1, "/");
      }
      return filename;
    }
    
    // loads and executes a file from the resource directory
    int State::doResourceFile(const std::string& inRelativePath)
    {
      return doFile(pathFromNamespace(inRelativePath));
    }
    
    // loads and executes a file
    int State::doFile(const std::string& inAbsolutePath)
    {
      std::ostringstream os;
      shared_ptr<resource::File> file = loader->load(inAbsolutePath);
      string data = file->str();
      md5wrapper md5;
      fileHashes[md5.getHashFromString(data)] = inAbsolutePath;
      return doString(data);
    }
    
    int State::doFile(const boost::shared_ptr<lost::resource::File>& inFile)
    {
      std::string data(inFile->str());
      md5wrapper  md5;
      
      fileHashes[md5.getHashFromString(data)] = inFile->location;
      return doString(data);
    }
    
    // executes the given string as a lua program
    int State::doString(const std::string& inData)
    {
      // execute the loaded file and handle errors
      int result = luaL_loadstring(state, inData.c_str());
      if (int err = (result || luabind::detail::pcall(state, 0, LUA_MULTRET)))
      {
        std::string errcode;
        switch(err)
        {
          case LUA_YIELD:errcode="LUA_YIELD";break;
          case LUA_ERRRUN:errcode="LUA_ERRUN";break;
          case LUA_ERRSYNTAX:errcode="LUA_ERRSYNTAX";break;
          case LUA_ERRMEM:errcode="LUA_ERRMEM";break;
          case LUA_ERRERR:errcode="LUA_ERRERR";break;
          default:errcode="??";break;
        }
        std::string errstring;
        const char* errstringc = lua_tostring(state, -1);
        lua_pop(state,1);
        if (errstringc != NULL)
        {
          errstring = errstringc;
          throw std::runtime_error(errcode +" : "+ errstring);
        }
        else
        {
          throw std::runtime_error(errcode);
        }
      }
      return result;
    }
    
  }
}
