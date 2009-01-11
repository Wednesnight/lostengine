#include "lost/lua/lua.h"
#include "lost/common/Logger.h"

#include "lost/lua/bindings/LostCommonLog.h"

using namespace luabind;
using namespace lost::common;

namespace lost
{
  namespace lua
  {

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
			lost::common::Logger::logMessage("DEBUG", "(Lua)", luabind_tostring(obj));
    }

    void info(luabind::object obj)
    {
			lost::common::Logger::logMessage("INFO", "(Lua)", luabind_tostring(obj));
    }

    void warn(luabind::object obj)
    {
			lost::common::Logger::logMessage("WARNING", "(Lua)", luabind_tostring(obj));
    }

    void error(luabind::object obj)
    {
			lost::common::Logger::logMessage("ERROR", "(Lua)", luabind_tostring(obj));
    }
  }
}

namespace lost
{
  namespace lua
  {
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
  }
}
