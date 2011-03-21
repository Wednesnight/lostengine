#include "lost/lua/bindings/ThirdpartyBoost.h"
#include "lost/lua/lua.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
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
  stream << e.path().filename();
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
          class_<path>("path")
            .def(constructor<>())
            .def(constructor<const std::string&>())
            .def(tostring(self))
            .def(self / other<std::string>())
            .def("remove_filename", &path::remove_filename)
            .def("filename", &path::filename)
            .def("stem", &path::stem)
            .def("extension", &path::extension),
        
          class_<directory_iterator>("directory_iterator"),
        
          class_<directory_entry>("directory_entry")
            .def(tostring(self))
            .def("status", (file_status (directory_entry::*)() const) &directory_entry::status),

          class_<file_status>("file_status"),

          def("is_directory", (bool(*)(file_status)) &is_directory)
        ]
       ];
    }

    void ThirdpartyBoost(lua_State* state)
    {
      ThirdpartyBoostFilesystem(state);
    }

  }
}
