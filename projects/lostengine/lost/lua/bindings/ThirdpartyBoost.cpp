#include "lost/lua/bindings/ThirdpartyBoost.h"
#include "lost/lua/lua.h"

#include <boost/filesystem.hpp>
#include <luabind/operator.hpp>

using namespace boost::filesystem;
using namespace luabind;

std::ostream& operator<<(std::ostream& stream, const path& p)
{
  stream << p.string();
  return stream;
}

std::ostream& operator<<(std::ostream& stream, const directory_entry& e)
{
  stream << e.path().string();
  return stream;
}

namespace lost
{
  namespace lua
  {

    void ThirdpartyBoostFilesystem(lua_State* state)
    {
      module(state, "boost")
      [
        namespace_("filesystem")
        [
          class_<boost::filesystem::path>("path")
            .def(constructor<>())
            .def(constructor<const std::string&>())
            .def(tostring(self))
            .def(self / other<std::string>()),
        
          class_<directory_iterator>("directory_iterator"),
        
          class_<directory_entry>("directory_entry")
            .def(tostring(self))
        ]
       ];
    }

    void ThirdpartyBoost(lua_State* state)
    {
      ThirdpartyBoostFilesystem(state);
    }

  }
}
