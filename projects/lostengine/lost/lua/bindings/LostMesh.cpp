#include "lost/lua/bindings/LostMesh.h"
#include "lost/lua/lua.h"

#include "lost/mesh/Line.h"
#include "lost/mesh/Loader.h"
#include "lost/mesh/Material.h"
#include "lost/mesh/Mesh.h"
#include "lost/mesh/Quad.h"
#include "lost/mesh/Rect.h"
#include "lost/mesh/Circular.h"
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
          class_<Line2D, Mesh, Line2DPtr>("Line2D")
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

    void LostMeshRect(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<Rect2D, Mesh, Rect2DPtr>("Rect2D")
            .def(constructor<const math::Rect&>())
            .def_readwrite("material", &Rect2D::material)
            .scope
            [
              def("create", &Rect2D::create)
            ],
          class_<FilledRect2D, Mesh, FilledRect2DPtr>("FilledRect2D")
            .def(constructor<const math::Rect&>())
            .def_readwrite("material", &FilledRect2D::material)
            .scope
            [
              def("create", &FilledRect2D::create)
            ],
          class_<RoundedRect2D, Mesh, RoundedRect2DPtr>("RoundedRect2D")
            .def(constructor<const math::Rect&, const double, const unsigned int>())
            .def_readwrite("material", &RoundedRect2D::material)
            .scope
            [
              def("create", &RoundedRect2D::create)
            ],
          class_<FilledRoundedRect2D, Mesh, FilledRoundedRect2DPtr>("FilledRoundedRect2D")
            .def(constructor<const math::Rect&, const double, const unsigned int>())
            .def_readwrite("material", &FilledRoundedRect2D::material)
            .scope
            [
              def("create", &FilledRoundedRect2D::create)
            ]
        ]
      ];
    }
    
    void LostMeshCircular(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<Circle2D, Mesh, Circle2DPtr>("Circle2D")
            .def(constructor<const double>())
            .def_readwrite("material", &Circle2D::material)
            .scope
            [
              def("create", &Circle2D::create)
            ],
          class_<FilledCircle2D, Mesh, FilledCircle2DPtr>("FilledCircle2D")
            .def(constructor<const double>())
            .def_readwrite("material", &FilledCircle2D::material)
            .scope
            [
              def("create", &FilledCircle2D::create)
            ],
          class_<Ellipse2D, Mesh, Ellipse2DPtr>("Ellipse2D")
            .def(constructor<const math::Vec2&>())
            .def_readwrite("material", &Ellipse2D::material)
            .scope
            [
              def("create", &Ellipse2D::create)
            ],
          class_<FilledEllipse2D, Mesh, FilledEllipse2DPtr>("FilledEllipse2D")
            .def(constructor<const math::Vec2&>())
            .def_readwrite("material", &FilledEllipse2D::material)
            .scope
            [
              def("create", &FilledEllipse2D::create)
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
            .def_readwrite("transform", &Mesh::transform)
            .def_readwrite("drawMode", &Mesh::drawMode),

          class_<Mesh2D, Mesh, MeshPtr>("Mesh2D")
            .def(constructor<>())
            .def_readwrite("material", &Mesh::material)
            .def_readwrite("transform", &Mesh::transform)
            .def_readwrite("drawMode", &Mesh::drawMode),

         class_<Mesh3D, Mesh, MeshPtr>("Mesh3D")
            .def(constructor<>())
            .def_readwrite("material", &Mesh::material)
            .def_readwrite("transform", &Mesh::transform)
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
          class_<Quad2D, Mesh, Quad2DPtr>("Quad2D")
            .def(constructor<>())
            .def(constructor<const math::Rect&>())
            .def(constructor<resource::FilePtr, bool>())
            .def(constructor<gl::TexturePtr, bool>())
            .def("updateSize", &Quad2D::updateSize)
            .def_readwrite("material", &Quad2D::material)
            .def_readwrite("transform", &Quad2D::transform)
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
      LostMeshRect(state);
      LostMeshCircular(state);
      LostMeshLoader(state);
      LostMeshMaterial(state);
      LostMeshQuad2D(state);
    }

  }
}
