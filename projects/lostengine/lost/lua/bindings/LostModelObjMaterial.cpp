#include "lost/lua/bindings/LostModelObjMaterial.h"
#include "lost/model/obj/Material.h"

using namespace luabind;
using namespace lost::model::obj;

LOST_LUA_EXPORT_BEGIN(LostModelObjMaterial)
{
  module(state, "lost")
  [
    namespace_("model")
    [
      namespace_("obj")
      [
        class_<Material, boost::shared_ptr<Material> >("Material")
        .def(constructor<>()) 
      ]
    ]
  ];
}
LOST_LUA_EXPORT_END
