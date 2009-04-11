#include <string>
#include <boost/shared_array.hpp>
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
  void install(State& inState, boost::shared_ptr<resource::Loader> inLoader)
  {
    const static string script =
    "local debug = _G.log.debug\n"
    "local setmetatable = _G.setmetatable\n"
    "local package = _G.package\n"
    "local loadstring = _G.loadstring\n"
    "local assert = _G.assert\n"
    "local environment = _G.environment\n"
    "\n"    
    "module('lost.lua.module')\n"
    "\n"
    "mt={}\n"
    "\n"
    "function customLoader(moduleName)\n"
    "  local f = assert(environment.interpreter:doResourceFile(moduleName..'.lua'))\n"
    "  return f\n"
    "end\n"
    "\n"
    "function customIndex(table, key)\n"
    "  return customLoader\n"
    "end\n"
    "\n"
    "function install()\n"
    "  mt.__index = customIndex\n"
    "  setmetatable(package.preload, mt)\n"
    "end\n"
    ;
    
    // execute script first so all vars are setup
    inState.doString(script);

    // setup loader
    object olost = globals(inState)["lost"];
    object olua = olost["lua"];
    object omodule = olua["module"];

    // find install function and call it
    object oinstall = omodule["install"];
    luabind::call_function<void>(oinstall);
  }
};
}
}
