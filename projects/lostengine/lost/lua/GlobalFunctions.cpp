#include <string>
#include "lost/lua/lua.h"
#include "lost/lua/State.h"

#include "lost/lua/GlobalFunctions.h"

using namespace std;
using namespace luabind;

namespace lost
{
  namespace lua
  {
    namespace GlobalFunctions
    {
      
      void install(State& inState)
      {
        /**
         * function using(namespace)
         */
        const static string script =
          " function using(namespace)"
          "   local targetTable = _G"
          "   local targetName = nil"
          "   for name in string.gmatch(namespace, \"[^%.]+\") do"
          "     if targetTable[name] ~= nil then"
          "       targetTable = targetTable[name]"
          "       targetName = name"
          "     else"
          "       error(namespace ..\" is not a valid namespace\", 2)"
          "     end"
          "   end"

          "   if targetTable ~= nil and targetName ~= nil then"
          "     local env = getfenv(2)"
          "     if env ~= nil then"
          "       env[targetName] = targetTable"
          "     else"
          "       error(\"Could not get environment of calling function\", 2)"
          "     end"
          "   end"
          " end";
        
        // execute script fragment
        inState.doString(script);
      }
      
    }
  }
}
