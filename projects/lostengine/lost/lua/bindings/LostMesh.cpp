#include "lost/lua/bindings/LostMesh.h"
#include "lost/lua/lua.h"

#include "lost/mesh/Line.h"
#include "lost/mesh/Loader.h"
#include "lost/mesh/Material.h"
#include "lost/mesh/Mesh.h"
#include "lost/mesh/Quad.h"
#include "lost/platform/shared_ptr.h"

using namespace luabind;
using namespace lost::mesh;

namespace lost
{
  namespace lua
  {
    MeshPtr castmesh(Mesh2DPtr m) { return lost::static_pointer_cast<Mesh>(m);}

    void LostMeshLine(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<Line2D, Mesh, MeshPtr>("Line2D")
            .def(constructor<>())
            .def(constructor<const math::Vec2&, const math::Vec2&>())
            .def_readwrite("material", &Line2D::material)
            .scope
            [
              def("create", (Line2DPtr(*)()) &Line2D::create),
              def("create", (Line2DPtr(*)(const math::Vec2&, const math::Vec2&)) &Line2D::create)
            ]
        ]
      ];
    }

    void LostMeshLoader(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<Loader, LoaderPtr>("Loader")
            .scope
            [
              def("obj", &lost::mesh::Loader::obj)
            ]
        ]
      ];
    }

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
            .def_readwrite("blend", &Material::blend)
            .def_readwrite("blendSrc", &Material::blendSrc)
            .def_readwrite("blendDest", &Material::blendDest)
            .scope
            [
              def("create", (MaterialPtr(*)())&Material::create)
            ]
        ]
      ];
    }

    void LostMeshMesh(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<Mesh, MeshPtr>("Mesh")
            .def(constructor<>())
            .def_readwrite("material", &Mesh::material)
            .def_readwrite("modelTransform", &Mesh::modelTransform)
            .def_readwrite("drawMode", &Mesh::drawMode),

          class_<Mesh2D, Mesh, MeshPtr>("Mesh2D")
            .def(constructor<>())
            .def_readwrite("material", &Mesh::material)
            .def_readwrite("modelTransform", &Mesh::modelTransform)
            .def_readwrite("drawMode", &Mesh::drawMode),

         class_<Mesh3D, Mesh, MeshPtr>("Mesh3D")
            .def(constructor<>())
            .def_readwrite("material", &Mesh::material)
            .def_readwrite("modelTransform", &Mesh::modelTransform)
            .def_readwrite("drawMode", &Mesh::drawMode)
        ]
      ];
    }

    void LostMeshQuad2D(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<Quad2D, Mesh, MeshPtr>("Quad2D")
            .def(constructor<>())
            .def(constructor<const math::Rect&>())
            .def(constructor<resource::FilePtr, bool>())
            .def(constructor<gl::TexturePtr, bool>())
            .def("updateSize", &Quad2D::updateSize)
            .def_readwrite("material", &Quad2D::material)
            .def_readwrite("modelTransform", &Quad2D::modelTransform)
            .scope
            [
              def("create", (Quad2DPtr(*)()) &Quad2D::create),
              def("create", (Quad2DPtr(*)(const math::Rect& inRect)) &Quad2D::create),
              def("create", (Quad2DPtr(*)(resource::FilePtr data, bool)) &Quad2D::create),
              def("create", (Quad2DPtr(*)(gl::TexturePtr tex, bool)) &Quad2D::create)
            ]
        ]
      ];
    }

    void LostMesh(lua_State* state)
    {
      // mesh first because following classes are based on it
      LostMeshMesh(state);
      LostMeshLine(state);
      LostMeshLoader(state);
      LostMeshMaterial(state);
      LostMeshQuad2D(state);
    }

  }
}
