#include "lost/lua/bindings/LostCommon.h"
#include "lost/lua/lua.h"
#include <luabind/operator.hpp>

#include "lost/common/Color.h"
#include "lost/common/FpsMeter.h"
#include "lost/common/Logger.h"

using namespace luabind;
using namespace lost::common;

namespace lost
{
  namespace lua
  {

    void LostCommonColor(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("common")
        [
          class_<Color, lost::shared_ptr<Color> >("Color")
            .def(constructor<>()) 
            .def(constructor<float, float, float>())
            .def(constructor<float, float, float, float>())
            .def(constructor<const Color&>())
            .def(self == other<Color>())
            .def(tostring(self))
            .property("r", (float(Color::*)() const)&Color::r, (void(Color::*)(float))&Color::r)
            .property("g", (float(Color::*)() const)&Color::g, (void(Color::*)(float))&Color::g)
            .property("b", (float(Color::*)() const)&Color::b, (void(Color::*)(float))&Color::b)
            .property("a", (float(Color::*)() const)&Color::a, (void(Color::*)(float))&Color::a)
        ]
      ];
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

    void LostCommonFpsMeter(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("common")
        [
          class_<FpsMeter, lost::shared_ptr<FpsMeter> >("FpsMeter")
            .def(constructor<>()) 
            .def("render", &FpsMeter::render)
            .def_readwrite("width", &FpsMeter::width)
            .def_readwrite("height", &FpsMeter::height)
            .def_readwrite("alpha", &FpsMeter::alpha)      
        ]
      ];
    }

    // from: http://osdir.com/ml/lang.lua.bind.user/2005-12/msg00034.html
    // NOTE: This is potentially buggy, we have to watch this and maybe reimplement it more restrictively
    // with object_cast, surrrounded by try/catch and extended error long in case of failure
    template <typename ValueWrapper>
    std::string luabind_tostring(ValueWrapper const& value_wrapper)
    {
      using namespace luabind;
      lua_State* interpreter = value_wrapper_traits<ValueWrapper>::interpreter(value_wrapper);
      value_wrapper_traits<ValueWrapper>::unwrap(interpreter, value_wrapper);
      detail::stack_pop pop(interpreter, 1);
      return std::string(lua_tostring(interpreter, -1), lua_strlen(interpreter, -1));
    }
    
    void debug(luabind::object obj)
    {
#if defined(LOST_LOGGER_ENABLE_DOUT)
			lost::common::Logger::logMessage("DEBUG", "(Lua)", luabind_tostring(obj));
#endif
    }
    
    void info(luabind::object obj)
    {
#if defined(LOST_LOGGER_ENABLE_IOUT)
			lost::common::Logger::logMessage("INFO", "(Lua)", luabind_tostring(obj));
#endif
    }
    
    void warn(luabind::object obj)
    {
#if defined(LOST_LOGGER_ENABLE_WOUT)
			lost::common::Logger::logMessage("WARNING", "(Lua)", luabind_tostring(obj));
#endif
    }
    
    void error(luabind::object obj)
    {
#if defined(LOST_LOGGER_ENABLE_EOUT)
			lost::common::Logger::logMessage("ERROR", "(Lua)", luabind_tostring(obj));
#endif
    }
    
    void LostCommonLog(lua_State* state)
    {
      module(state, "log")
      [
        def("debug", &debug),
        def("info", &info),
        def("warn", &warn),
        def("error", &error)
      ];
    }

    void LostCommon(lua_State* state)
    {
      LostCommonColor(state);
      LostCommonFpsMeter(state);
      LostCommonLog(state);
    }

  }
}
