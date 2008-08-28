#ifndef LOST_LUA_STATE_H
#define LOST_LUA_STATE_H

#include "lost/lua/lua.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "lost/platform/Platform.h"
#include "lost/common/Logger.h"

namespace lost
{
  namespace lua
  {

    int errorCallback(lua_State* state);

    struct State
    {
      State(bool callLuabindOpen=true, bool doOpenLibs=true, bool doInitPackagePath=true)
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
        luabind::set_pcall_callback(errorCallback);
      }

      ~State()
      {
        lua_close(state);
      }

      void handleError()
      {
        lua_Debug debug;
        lua_getstack(state, 0, &debug);
        lua_getinfo(state, "Sln", &debug);
        
        // old error message
        std::string err = lua_tostring(state, -1);
        EOUT(err);
        lua_pop(state, 1);
        
        std::stringstream msg;
        msg << "in " << debug.what;
        if (debug.namewhat != 0) msg << " " << debug.namewhat;
        else msg << " unknown";
        if (debug.name != 0) msg << " " << debug.name;
        if (debug.currentline >= 0) msg  << " (line " << debug.currentline << ")";
        
        // print out current func
        EOUT(msg.str());
        
        // get call stack
        unsigned int idx = 1;
        while (idx <= callstackSize && lua_getstack(state, idx++, &debug) == 1)
        {
          lua_getinfo(state, "Sln", &debug);
          msg.str("");
          msg << "called from " << debug.what;
          if (debug.namewhat != "") msg << " " << debug.namewhat;
          else msg << " unknown";
          if (debug.name != 0) msg << " " << debug.name;
          if (debug.currentline >= 0) msg  << " (line " << debug.currentline << ")";
          
          // print out info
          EOUT(msg.str());
        }
      }
      
      // cast operator to original lua_State* value
      operator lua_State*() { return state; }

      // replaces package load path with path to resource dir
      void initPackagePath()
      {
        std::string basePath=lost::platform::getResourcePath()+"/";
        std::string packageSearchPattern = basePath+"?;"+basePath+"?.lua";
        luabind::globals(state)["package"]["path"] = packageSearchPattern;
      }

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

      // loads and executes a file from the resource directory
      void doResourceFile(const std::string& inRelativePath)
      {
        doFile(lost::platform::buildResourcePath(inRelativePath));
      }

      // loads and executes a file
      void doFile(const std::string& inAbsolutePath)
      {
        std::ostringstream os;
        std::ifstream file(inAbsolutePath.c_str());
        if(!file) throw std::runtime_error("couldn't load file: '"+inAbsolutePath+"'");
        os << file.rdbuf();
        std::string data = os.str();
        doString(data);
      }

      // executes the given string as a lua program
      void doString(const std::string& inData)
      {
        // execute the loaded file and handle errors
        if (int err = luaL_dostring(state, inData.c_str()))
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
      }

      // returns memory usage of the lua state in kilobytes
      int memUsage() { return lua_getgccount(state); }


      lua_State* state;

      int callstackSize;
    };

  }
}


#endif
