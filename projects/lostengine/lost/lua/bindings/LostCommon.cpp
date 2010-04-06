#include "lost/lua/bindings/LostCommon.h"
#include "lost/lua/lua.h"
#include <luabind/operator.hpp>

#include "lost/common/Color.h"
#include "lost/common/Data.h"
#include "lost/common/Logger.h"
#include "lost/common/ColorPoint.h"
#include "lost/common/ColorGradient.h"
#include "lost/lua/State.h"

using namespace luabind;
using namespace std;
using namespace lost::common;

namespace lost
{
  namespace lua
  {

    void LostCommonData(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("common")
        [
          class_<Data>("Data")
            .def_readwrite("size", &Data::size)
            .def("str", &Data::str)
        ]
      ];
    }

    void LostCommonColor(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("common")
        [
          class_<Color>("Color")
            .def(constructor<>()) 
            .def(constructor<float, float, float>())
            .def(constructor<float, float, float, float>())
            .def(constructor<const Color&>())
            .def(self == other<Color>())
            .def(tostring(self))
            .def(self * other<Color>())
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

    void LostCommonColorPoint(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("common")
        [
          class_<ColorPoint>("ColorPoint")
            .def(constructor<>()) 
            .def(constructor<float, const Color&>())
        ]
      ];
    }

    void LostCommonColorGradient(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("common")
        [
          class_<ColorGradient>("ColorGradient")
          .def("add", &ColorGradient::add)
          .scope
          [
            def("create", &ColorGradient::create)
          ]
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
    
    std::string getFilenameFuncnameLine(lua_State* state)
    {
      State* ls = State::stateFromState(state);
      return ls->getFilenameFuncnameLine();
    }
    
    void debug(luabind::object obj)
    {
#if defined(LOST_LOGGER_ENABLE_DOUT)
			lost::common::Logger::logMessage("DEBUG", getFilenameFuncnameLine(obj.interpreter()), luabind_tostring(obj));
#endif
    }
    
    void info(luabind::object obj)
    {
#if defined(LOST_LOGGER_ENABLE_IOUT)
			lost::common::Logger::logMessage("INFO", getFilenameFuncnameLine(obj.interpreter()), luabind_tostring(obj));
#endif
    }
    
    void warn(luabind::object obj)
    {
#if defined(LOST_LOGGER_ENABLE_WOUT)
			lost::common::Logger::logMessage("WARNING", getFilenameFuncnameLine(obj.interpreter()), luabind_tostring(obj));
#endif
    }
    
    void error(luabind::object obj)
    {
#if defined(LOST_LOGGER_ENABLE_EOUT)
			lost::common::Logger::logMessage("ERROR", getFilenameFuncnameLine(obj.interpreter()), luabind_tostring(obj));
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
      LostCommonColorPoint(state);
      LostCommonColorGradient(state);
      LostCommonLog(state);
      LostCommonData(state);
    }

  }
}
