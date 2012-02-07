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

#include "lost/lua/bindings/LostProfiler.h"
#include "lost/lua/lua.h"
#include "lost/profiler/Blackbox.h"

#include <slub/slub.h>

namespace lost
{
  namespace lua
  {

    void bb_inc(const string& name) { BB_INC(name); }
    void bb_dec(const string& name) { BB_DEC(name); }
    void bb_set_clear(const string& name, bool flag) { BB_SET_CLEAR(name,flag); }
    void bb_set(const string& name,float val) { BB_SET(name,val); }

    void LostProfilerBlackbox(lua_State* state)
    {
      slub::package(state,"lost").package("profiler")
        .function("bb_inc",&bb_inc)
        .function("bb_dec",&bb_dec)
        .function("bb_set_clear",&bb_set_clear)
        .function("bb_set",&bb_set);
    }

    void LostProfilerBBCount(lua_State* state)
    {
      slub::package(state,"lost").package("profiler")
        .clazz<lost::profiler::BBCount>("BBCount")
          .constructor<const string&>();
    }
    
    void LostProfiler(lua_State* state)
    {
      LostProfilerBlackbox(state);
      LostProfilerBBCount(state);
    }

  }
}
