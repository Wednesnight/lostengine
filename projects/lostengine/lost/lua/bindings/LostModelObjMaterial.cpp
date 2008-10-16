#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/model/obj/Material.h"

#include "lost/lua/bindings/LostModelObjMaterial.h"

using namespace luabind;
using namespace lost::model::obj;

namespace lost
{
  namespace lua
  {
    void LostModelObjMaterial(lua_State* state)
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
  }
}
