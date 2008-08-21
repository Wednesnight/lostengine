#include "lost/lua/Luabindings.h"
#include "lost/common/Logger.h"

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
      dout << "(Lua) " << luabind_tostring(obj) << lendl;
    }

    void info(luabind::object obj)
    {
      iout << "(Lua) " << luabind_tostring(obj) << lendl;
    }

    void warn(luabind::object obj)
    {
      wout << "(Lua) " << luabind_tostring(obj) << lendl;
    }

    void error(luabind::object obj)
    {
      eout << "(Lua) " << luabind_tostring(obj) << lendl;
    }
  }
}

LOST_LUA_EXPORT_BEGIN(LuaLostCommonLog)
{
  module(state, "log")
  [
    def("debug", &debug),
    def("info", &info),
    def("warn", &warn),
    def("error", &error)
  ];
}
LOST_LUA_EXPORT_END
