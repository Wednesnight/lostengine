#include "lost/lua/lua.h"
#include "lost/model/obj/Parser.h"

#include "lost/lua/bindings/LostModelObjParser.h"

using namespace luabind;
using namespace lost::model::obj;

namespace lost
{
  namespace lua
  {
    void LostModelObjParser(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("model")
        [
          namespace_("obj")
          [
            class_<Parser, lost::shared_ptr<Parser> >("Parser")
            .def(constructor<lost::shared_ptr<resource::Loader> >())
            .def("parseMesh", &Parser::parseMesh)
          ]
        ]
      ];
    }
  }
}
