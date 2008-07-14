#include "lost/lua/ModuleLoader.h"
#include <string>

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
    "\n"    
    "module('lost.lua.module')\n"
    "\n"
    "mt={}\n"
    "resourceLoader=nil\n"
    "\n"
    "function customLoader(moduleName)\n"
    "  debug('trying to load module: '..moduleName)\n"
    "  local file = resourceLoader:load(moduleName..'.lua')\n"
    "  local data = file:str()\n"
    "  local f = assert(loadstring(data))\n"
    "  f()"
    "  return f\n"
    "end\n"
    "\n"
    "function customIndex(table, key)\n"
    "  debug('package.preload accessed, returning custom module loader')\n"
    "  return customLoader\n"
    "end\n"
    "\n"
    "function install()\n"
    "  debug('installing custom module loader')\n"
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
    omodule["resourceLoader"] = inLoader;

    // find install function and call it
    object oinstall = omodule["install"];
    luabind::call_function<void>(oinstall);
  }
};
}
}
