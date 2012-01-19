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

#ifndef LOST_LUA_STATE_H
#define LOST_LUA_STATE_H

#include "lost/common/Disallow.h"
#include "lost/common/Data.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/lua/forward.h"

#include <slub/globals.h>

namespace lost
{
  namespace lua
  {
    struct State 
    {
      State(resource::LoaderPtr inLoader = resource::LoaderPtr(new resource::DefaultLoader));
      ~State();

      string getScriptFilename(const string& scriptContent, const string& defaultName);
      string getScriptSource(lua_Debug& debug);
      static State* stateFromState(lua_State* state);
      static int errorHandler(lua_State* state);
      int handleError();

      // cast operator to original lua_State* value
      operator lua_State*() { return state; }

      // adds all standard libs to the state of the interpreter
      void openLibs();
      // or fine grained functions if you only want to add single libs
      void openBaseLib();
      void openDebugLib();
      void openIoLib();
      void openMathLib();
      void openPackageLib();
      void openStringLib();
      void openTableLib();
      void openOsLib();

      void addScriptPathEntry(const string& inScript, const string& inAbsolutePath);

      // convert naming convention "lost.lua..." to path "lost/lua/..."
      string pathFromNamespace(const string& inNamespace);

      // loads and executes a file from the resource directory
      int doResourceFile(const string& inRelativePath);

      // loads and executes a file
      int doFile(const string& inAbsolutePath);

      // executes the given string as a lua program
      int doString(const string& inData);

      // returns memory usage of the lua state in kilobytes
      int memUsage() { return lua_getgccount(state); }

      string getFilenameFuncnameLine(); // formatted for logging

      lua_State* state;
      int callstackSize;
      map<string, string> fileHashes;
      resource::LoaderPtr loader;
      slub::globals globals;
    private:
      DISALLOW_COPY_AND_ASSIGN(State);
      
    };

  }
}


#endif
