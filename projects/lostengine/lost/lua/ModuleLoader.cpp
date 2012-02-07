/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/lua/lua.h"
#include "lost/lua/State.h"

#include "lost/lua/ModuleLoader.h"

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
