#include "lost/lua/LuaBindings.h"
#include "lost/model/MaterialOBJ.h"

using namespace luabind;
using namespace lost::model;

LOST_LUA_EXPORT_BEGIN(LuaLostModelMaterialOBJ)
{
  module(state, "lost")
  [
    namespace_("model")
    [
      class_<MaterialOBJ, boost::shared_ptr<MaterialOBJ> >("MaterialOBJ")
      .def(constructor<>()) 
    ]
  ];
}
LOST_LUA_EXPORT_END
