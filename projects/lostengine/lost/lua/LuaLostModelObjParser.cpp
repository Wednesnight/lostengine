#include "lost/lua/LuaBindings.h"
#include "lost/model/obj/Parser.h"

using namespace luabind;
using namespace lost::model::obj;

LOST_LUA_EXPORT_BEGIN(LuaLostModelObjParser)
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
LOST_LUA_EXPORT_END
