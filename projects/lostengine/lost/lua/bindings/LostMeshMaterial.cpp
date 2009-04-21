#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/mesh/Material.h"

#include "lost/lua/bindings/LostMeshMaterial.h"

using namespace luabind;
using namespace lost::mesh;

namespace lost
{
  namespace lua
  {
    void LostMeshMaterial(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<Material, MaterialPtr >("Material")
            .def(constructor<>())
            .def_readwrite("shader", &Material::shader)
            .def_readwrite("color", &Material::color)
            .scope
            [
              def("create", (MaterialPtr(*)())&Material::create)
            ]
        ]
      ];
    }
  }
}
