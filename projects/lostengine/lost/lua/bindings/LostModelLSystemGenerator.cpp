#include <boost/shared_ptr.hpp>
#include "lost/lua/State.h"
#include "lost/model/lsystem/Generator.h"

#include "lost/lua/bindings/LostModelLSystemGenerator.h"

using namespace luabind;
using namespace lost::model;
using namespace lost::model::lsystem;

namespace lost
{
  namespace lua
  {
    void LostModelLSystemGenerator(lost::lua::State& state)
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
  }
}
