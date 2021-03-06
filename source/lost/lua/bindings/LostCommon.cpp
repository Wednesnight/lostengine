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

#include "lost/common/io.h"
#include "lost/lua/bindings/LostCommon.h"

#include "lost/lua/lostlua.h"
#include "lost/lua/State.h"

#include "lost/common/Color.h"
#include "lost/common/Data.h"
#include "lost/common/Logger.h"
#include "lost/common/ColorPoint.h"
#include "lost/common/ColorGradient.h"
#include "lost/common/Hash.h"

#include <slub/slub.h>

using namespace lost::common;

namespace lost
{
  namespace lua
  {

    void LostCommonData(lua_State* state)
    {
      slub::package(state, "lost").package("common")
        .clazz<Data>("Data")
          .method("str", &Data::str)
          .field("size", &Data::size)
          .function("create", &Data::create);
    }

    void LostCommonColor(lua_State* state)
    {
      slub::package(state, "lost").package("common")
        .clazz<Color>("Color")
          .constructor()
          .constructor<float, float, float>()
          .constructor<float, float, float, float>()
          .constructor<const Color&>()
          .eq()
          .tostring()
          .mul<Color, Color>()
          .mul<Color, float>()
          .method("premultiplied", &Color::premultiplied)
          .field("r", (float(Color::*)() const)&Color::r, (void(Color::*)(float))&Color::r)
          .field("g", (float(Color::*)() const)&Color::g, (void(Color::*)(float))&Color::g)
          .field("b", (float(Color::*)() const)&Color::b, (void(Color::*)(float))&Color::b)
          .field("a", (float(Color::*)() const)&Color::a, (void(Color::*)(float))&Color::a);
      /*
       globals(state)["lost"]["common"]["transparentColor"] = Color(transparentColor);
       globals(state)["lost"]["common"]["whiteColor"]       = Color(whiteColor);
       globals(state)["lost"]["common"]["blackColor"]       = Color(blackColor);
       globals(state)["lost"]["common"]["redColor"]         = Color(redColor);
       globals(state)["lost"]["common"]["greenColor"]       = Color(greenColor);
       globals(state)["lost"]["common"]["blueColor"]        = Color(blueColor);
       globals(state)["lost"]["common"]["yellowColor"]      = Color(yellowColor);
       */
    }

    void LostCommonColorPoint(lua_State* state)
    {
      slub::package(state, "lost").package("common")
        .clazz<ColorPoint>("ColorPoint")
          .constructor()
          .constructor<float, const Color&>();
    }

    void LostCommonColorGradient(lua_State* state)
    {
      slub::package(state, "lost").package("common")
        .clazz<ColorGradient>("ColorGradient")
          .method("add", &ColorGradient::add)
          .function("create", &ColorGradient::create);
    }

    string tostring(lua_State* state, const slub::reference& obj)
    {
      slub::converter<const slub::reference&>::push(state, obj);
      string result;
      if (lua_getmetatable(state, -1)) {  /* does it have a metatable? */
        lua_getfield(state, -1, "__tostring");
        if (lua_type(state, -1) == LUA_TFUNCTION) {
          lua_pushvalue(state, -3);
          lua_call(state, 1, 1);
          result = string(luaL_checkstring(state, -1));
        }
        lua_pop(state, 2);
      }
      if (result.size() == 0) {
        if (lua_isstring(state, -1)) {
          result = string(luaL_checkstring(state, -1));
        }
        else {
          result = luaL_typename(state, -1);
        }
      }
      lua_pop(state, 1);
      return result;
    }
    
    string getFilenameFuncnameLine(lua_State* state)
    {
      State* ls = State::stateFromState(state);
      return ls->getFilenameFuncnameLine();
    }
    
    void debug(const slub::reference& obj, lua_State* state)
    {
#if defined(LOST_LOGGER_ENABLE_DOUT)
			lost::common::Logger::logMessage("DEBUG", getFilenameFuncnameLine(state), tostring(state, obj));
#endif
    }
    
    void info(const slub::reference& obj, lua_State* state)
    {
#if defined(LOST_LOGGER_ENABLE_IOUT)
			lost::common::Logger::logMessage("INFO", getFilenameFuncnameLine(state), tostring(state, obj));
#endif
    }
    
    void warn(const slub::reference& obj, lua_State* state)
    {
#if defined(LOST_LOGGER_ENABLE_WOUT)
			lost::common::Logger::logMessage("WARNING", getFilenameFuncnameLine(state), tostring(state, obj));
#endif
    }
    
    void error(const slub::reference& obj, lua_State* state)
    {
#if defined(LOST_LOGGER_ENABLE_EOUT)
			lost::common::Logger::logMessage("ERROR", getFilenameFuncnameLine(state), tostring(state, obj));
#endif
    }
    
    void LostCommonLog(lua_State* state)
    {
      slub::package(state, "log")
        .function("debug", &debug)
        .function("info", &info)
        .function("warn", &warn)
        .function("error", &error);
    }
  
    void LostCommonHash(lua_State* state)
    {
      slub::package(state, "lost").package("common")
        .function("djb2Hash",&lost::common::djb2Hash);
    }

    void LostCommon(lua_State* state)
    {
      LostCommonColor(state);
      LostCommonColorPoint(state);
      LostCommonColorGradient(state);
      LostCommonLog(state);
      LostCommonData(state);
      LostCommonHash(state);
    }

  }
}
