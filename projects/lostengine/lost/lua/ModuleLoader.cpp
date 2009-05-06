#include <string>
#include "lost/lua/lua.h"
#include "lost/lua/State.h"

#include "lost/lua/ModuleLoader.h"

using namespace std;
using namespace luabind;

namespace lost
{
  namespace lua
  {
    namespace ModuleLoader
    {

      void install(State& inState, lost::shared_ptr<resource::Loader> inLoader)
      {
        const static string script = "local mt = "
                                     "{ "
                                     "  __index = function(self, key) "
                                     "    local mt = getmetatable(self) or {} "
                                     "    return mt.load "
                                     "  end, "
                                     " "
                                     "  load = function(moduleName) "
                                     "    return assert(lost.lua.doResourceFile(moduleName..'.lua')) "
                                     "  end "
                                     "} "
                                     "setmetatable(package.preload, mt)";
        
        // setup loader
        inState.globals["loader"] = inLoader;
        // execute script fragment
        inState.doString(script);
      }

    }
  }
}
