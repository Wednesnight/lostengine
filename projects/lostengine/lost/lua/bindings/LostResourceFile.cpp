#include "lost/lua/bindings/LostResourceFile.h"
#include "lost/resource/File.h"

using namespace luabind;
using namespace lost::resource;

LOST_LUA_EXPORT_BEGIN(LostResourceFile)
{
  module(state, "lost")
  [
    namespace_("resource")
    [
      class_<File, boost::shared_ptr<File> >("File")
        .def(constructor<>())    
         .def_readwrite("size",     &File::size)
         .def_readwrite("location",     &File::location)
         .def("str", &File::str)
    ]
  ];
}
LOST_LUA_EXPORT_END
