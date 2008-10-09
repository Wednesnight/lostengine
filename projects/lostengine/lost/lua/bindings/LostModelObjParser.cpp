#include <boost/shared_ptr.hpp>
#include "lost/lua/State.h"
#include "lost/model/obj/Parser.h"

#include "lost/lua/bindings/LostModelObjParser.h"

using namespace luabind;
using namespace lost::model::obj;

namespace lost
{
  namespace lua
  {
    void LostModelObjParser(lost::lua::State& state)
    {
      module(state, "lost")
      [
        namespace_("model")
        [
          namespace_("obj")
          [
            class_<Parser, boost::shared_ptr<Parser> >("Parser")
            .def(constructor<boost::shared_ptr<resource::Loader> >())
            .def("parseMesh", &Parser::parseMesh)
          ]
        ]
      ];
    }
  }
}
