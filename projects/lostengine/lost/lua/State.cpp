#include "lost/lua/State.h"
#include <hashlibpp.h>
#include "lost/platform/Platform.h"
#include "lost/common/Logger.h"
#include <stdexcept>

namespace lost
{
  namespace lua
  {
    
    State::State(bool callLuabindOpen, bool doOpenLibs, bool doInitPackagePath)
    : callstackSize(10)
    {
      state = luaL_newstate();
      if(callLuabindOpen) luabind::open(state);
      if(doOpenLibs) openLibs();
      if(doInitPackagePath)
      {
        openPackageLib(); // we need to make sure the package library is loaded otherwise we'll crash
        initPackagePath();
      }
      
      // set our own error callback
      int errorCallback(lua_State* state);
      luabind::set_pcall_callback(errorCallback);
    }
    
    State::~State()
    {
      lua_close(state);
    }
    
    std::string State::getScriptSource(lua_Debug& debug)
    {
      std::string result("source: ");
      md5wrapper  md5;
      std::string file(md5.getHashFromString(debug.source));
      if (fileHashes.find(file) != fileHashes.end()) result.append(fileHashes[file]);
      else result.append(debug.short_src);
      
      return result;
    }
    
    int State::handleError()
    {
      lua_Debug debug;
      lua_getstack(state, 1, &debug);
      lua_getinfo(state, "Sln", &debug);
      
      // get call stack
      std::string offset;
      for (unsigned int idx = callstackSize; idx > 0; --idx)
      {
        std::stringstream msg;
        if (lua_getstack(state, idx, &debug) == 1)
        {
          lua_getinfo(state, "Sln", &debug);
          msg << offset << "-> " << debug.what;
          if (debug.namewhat != "") msg << " " << debug.namewhat;
          else msg << " unknown";
          if (debug.name != 0) msg << " " << debug.name;
          if (debug.currentline >= 0) msg  << " (" << getScriptSource(debug) << ", line " << debug.currentline << ")";
          
          // print out info
          EOUT(msg.str());

          // format next message
          offset.append("  ");
        }
      }
      
      // lua error message
      std::string error = lua_tostring(state, -1);
      lua_pop(state, 1);
      EOUT(offset << error);

      return 1;
    }
    
    // replaces package load path with path to resource dir
    void State::initPackagePath()
    {
      std::string basePath=lost::platform::getResourcePath()+"/";
      std::string packageSearchPattern = basePath+"?;"+basePath+"?.lua";
      luabind::globals(state)["package"]["path"] = packageSearchPattern;
    }
    
    // loads and executes a file from the resource directory
    int State::doResourceFile(const std::string& inRelativePath)
    {
      return doFile(lost::platform::buildResourcePath(inRelativePath));
    }
    
    // loads and executes a file
    int State::doFile(const std::string& inAbsolutePath)
    {
      std::ostringstream os;
      std::ifstream file(inAbsolutePath.c_str());
      if(!file) throw std::runtime_error("couldn't load file: '"+inAbsolutePath+"'");
      os << file.rdbuf();
      std::string data = os.str();
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
        std::string errstring = lua_tostring(state, -1);
        lua_pop(state,1);
        throw std::runtime_error(errcode+" : "+errstring);
      }
      return result;
    }
    
  }
}
