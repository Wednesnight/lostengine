#include "lost/lua/lua.h"
#include "lost/lua/State.h"

#include "lost/lua/ModuleLoader.h"

using namespace luabind;

namespace lost
{
  namespace lua
  {
    namespace ModuleLoader
    {

      void install(State& inState)
      {
        const static string script = "local function moduleloader__index(self,key)\n"
                                     "    local mt = getmetatable(self) or {} \n"
                                     "    return mt.load \n"
                                     "end\n"
                                     "local function moduleloader_load(moduleName)\n"
                                     "    return assert(lost.lua.doResourceFile(moduleName..'.lua')) \n"
                                     "  end \n"
                                     " \n"
                                     "local mt = \n"
                                     "{ \n"
                                     "  __index = moduleloader__index, \n"
                                     " \n"
                                     "  load = moduleloader_load \n"
                                     "} \n"
                                     "setmetatable(package.preload, mt)\n";
        inState.addScriptPathEntry(script, "ModuleLoader.cpp");
        // execute script fragment
        inState.doString(script);
      }

    }
  }
}
