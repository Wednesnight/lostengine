#include <sstream>
#include "lost/lua/lua.h"
#include "lost/common/Logger.h"

using namespace luabind;

namespace lost
{
  namespace lua
  {

    int errorCallback(lua_State* state)
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

      // get call stack (depth: 10)
      unsigned int idx = 1;
      while (idx <= 10 && lua_getstack(state, idx++, &debug) == 1)
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

      return 1;
    }

  }
}
