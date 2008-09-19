#include "lost/lua/bindings/LostModelLSystemGenerator.h"
#include "lost/model/lsystem/Generator.h"

using namespace luabind;
using namespace lost::model;
using namespace lost::model::lsystem;

LOST_LUA_EXPORT_BEGIN(LostModelLSystemGenerator)
{
  module(state, "lost")
  [
    namespace_("model")
    [
      namespace_("lsystem")
      [
        class_<Generator, boost::shared_ptr<Generator> >("Generator")
        .def(constructor<>()) 
        .def("generate", &Generator::generate)
      ]
    ]
  ];
}
LOST_LUA_EXPORT_END

