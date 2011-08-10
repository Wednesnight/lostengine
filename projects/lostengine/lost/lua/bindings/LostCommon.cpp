#include "lost/lua/bindings/LostCommon.h"

#include "lost/lua/lua.h"
#include "lost/lua/State.h"

#include "lost/common/Color.h"
#include "lost/common/io.h"
#include "lost/common/Data.h"
#include "lost/common/Logger.h"
#include "lost/common/ColorPoint.h"
#include "lost/common/ColorGradient.h"
#include "lost/common/Hash.h"

#include <slub/slub.h>

using namespace lost::common;
using namespace slub;

namespace lost
{
  namespace lua
  {

    void LostCommonData(lua_State* state)
    {
      package(state, "lost").package("common")
        .clazz<Data>("Data")
          .method("str", &Data::str)
          .field("size", &Data::size)
          .function("create", &Data::create);
    }

    void LostCommonColor(lua_State* state)
    {
      package(state, "lost").package("common")
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
      package(state, "lost").package("common")
        .clazz<ColorPoint>("ColorPoint")
          .constructor()
          .constructor<float, const Color&>();
    }

    void LostCommonColorGradient(lua_State* state)
    {
      package(state, "lost").package("common")
        .clazz<ColorGradient>("ColorGradient")
          .method("add", &ColorGradient::add)
          .function("create", &ColorGradient::create);
    }

    string tostring(lua_State* state, const reference& obj)
    {
      converter<const reference&>::push(state, obj);
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
    
    void debug(const reference& obj, lua_State* state)
    {
#if defined(LOST_LOGGER_ENABLE_DOUT)
			lost::common::Logger::logMessage("DEBUG", getFilenameFuncnameLine(state), tostring(state, obj));
#endif
    }
    
    void info(const reference& obj, lua_State* state)
    {
#if defined(LOST_LOGGER_ENABLE_IOUT)
			lost::common::Logger::logMessage("INFO", getFilenameFuncnameLine(state), tostring(state, obj));
#endif
    }
    
    void warn(const reference& obj, lua_State* state)
    {
#if defined(LOST_LOGGER_ENABLE_WOUT)
			lost::common::Logger::logMessage("WARNING", getFilenameFuncnameLine(state), tostring(state, obj));
#endif
    }
    
    void error(const reference& obj, lua_State* state)
    {
#if defined(LOST_LOGGER_ENABLE_EOUT)
			lost::common::Logger::logMessage("ERROR", getFilenameFuncnameLine(state), tostring(state, obj));
#endif
    }
    
    void LostCommonLog(lua_State* state)
    {
      package(state, "log")
        .function("debug", &debug)
        .function("info", &info)
        .function("warn", &warn)
        .function("error", &error);
    }
  
    void LostCommonHash(lua_State* state)
    {
      package(state, "lost").package("common")
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
