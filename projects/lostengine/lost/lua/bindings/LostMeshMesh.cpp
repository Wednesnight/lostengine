#include "lost/lua/lua.h"
#include "lost/mesh/Mesh.h"

#include "lost/lua/bindings/LostMeshMesh.h"

using namespace luabind;
using namespace lost::mesh;

namespace lost
{
  namespace lua
  {
    void LostMeshMesh(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<Mesh, MeshPtr >("Mesh")
            .def(constructor<>())
            .def_readwrite("material", &Mesh::material)
            .def_readwrite("modelTransform", &Mesh::modelTransform)
            .def_readwrite("drawMode", &Mesh::drawMode),
          class_<Mesh3D, Mesh3DPtr >("Mesh3D")
            .def_readwrite("material", &Mesh::material)
            .def_readwrite("modelTransform", &Mesh::modelTransform)
            .def_readwrite("drawMode", &Mesh::drawMode),
          def("createFromOBJ", &lost::mesh::createFromOBJ)
        ]
      ];
    }
  }
}
