#include "lost/lua/LuaBindings.h"
#include "lost/model/parser/ParserOBJ.h"

using namespace luabind;
using namespace lost::model::parser;

LOST_LUA_EXPORT_BEGIN(LuaLostModelParserParserOBJ)
{
  module(state, "lost")
  [
    namespace_("model")
    [
      namespace_("parser")
      [
        class_<ParserOBJ, boost::shared_ptr<ParserOBJ> >("ParserOBJ")
        .def(constructor<boost::shared_ptr<resource::Loader> >())
        .def("parseMesh", &ParserOBJ::parseMesh)
      ]
    ]
  ];
}
LOST_LUA_EXPORT_END
