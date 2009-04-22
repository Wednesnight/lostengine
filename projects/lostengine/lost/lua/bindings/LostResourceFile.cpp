#include "lost/lua/lua.h"
#include "lost/resource/File.h"

#include "lost/lua/bindings/LostResourceFile.h"

using namespace luabind;
using namespace lost::resource;

namespace lost
{
  namespace lua
  {
    void LostResourceFile(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("resource")
        [
          class_<File, lost::shared_ptr<File> >("File")
            .def(constructor<>())    
             .def_readwrite("size",     &File::size)
             .def_readwrite("location",     &File::location)
             .def("str", &File::str)
        ]
      ];
    }
  }
}
