#include "lost/lua/bindings/LostMesh.h"
#include "lost/lua/lua.h"

#include "lost/mesh/Line.h"
#include "lost/mesh/Loader.h"
#include "lost/mesh/Material.h"
#include "lost/mesh/Mesh.h"
#include "lost/mesh/Quad.h"
#include "lost/mesh/ScaleGrid.h"
#include "lost/mesh/Rect.h"
#include "lost/mesh/Circular.h"
#include "lost/platform/shared_ptr.h"

using namespace luabind;
using namespace lost::mesh;

namespace lost
{
  namespace lua
  {
    void LostMeshLine(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<Line2D, Mesh>("Line2D")
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
          class_<Rect2D, Mesh>("Rect2D")
            .def("updateSize", &Rect2D::updateSize)
            .scope
            [
              def("create", &Rect2D::create)
            ],
          class_<FilledRect2D, Mesh>("FilledRect2D")
            .scope
            [
              def("create", &FilledRect2D::create)
            ],
          class_<RoundedRect2D, Mesh>("RoundedRect2D")
            .scope
            [
              def("create", &RoundedRect2D::create)
            ],
          class_<FilledRoundedRect2D, Mesh>("FilledRoundedRect2D")
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
          class_<Circle2D, Mesh>("Circle2D")
            .def_readwrite("material", &Circle2D::material)
            .scope
            [
              def("create", &Circle2D::create)
            ],
          class_<FilledCircle2D, Mesh>("FilledCircle2D")
            .def_readwrite("material", &FilledCircle2D::material)
            .scope
            [
              def("create", &FilledCircle2D::create)
            ],
          class_<Ellipse2D, Mesh>("Ellipse2D")
            .def_readwrite("material", &Ellipse2D::material)
            .scope
            [
              def("create", &Ellipse2D::create)
            ],
          class_<FilledEllipse2D, Mesh>("FilledEllipse2D")
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
          class_<Loader>("Loader")
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
          class_<Material>("Material")
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
          class_<Mesh>("Mesh")
            .def_readwrite("material", &Mesh::material)
            .def_readwrite("transform", &Mesh::transform)
            .def_readwrite("drawMode", &Mesh::drawMode),
          class_<Mesh2D, Mesh>("Mesh2D"),
         class_<Mesh3D, Mesh>("Mesh3D")
        ]
      ];
    }

    void LostMeshQuad2D(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<Quad2D, Mesh>("Quad2D")
            .def("updateSize", &Quad2D::updateSize)
            .scope
            [
              def("create", (Quad2DPtr(*)()) &Quad2D::create),
              def("create", (Quad2DPtr(*)(const math::Rect& inRect)) &Quad2D::create),
              def("create", (Quad2DPtr(*)(common::DataPtr data, bool)) &Quad2D::create),
              def("create", (Quad2DPtr(*)(gl::TexturePtr tex, bool)) &Quad2D::create)
            ]
        ]
      ];
    }

    void LostMeshScaleGrid2D(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<ScaleGrid2D, Mesh>("ScaleGrid2D")
          .def("updateSize", &ScaleGrid2D::updateSize)
          .scope
          [
            def("create", &ScaleGrid2D::create)            
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
      LostMeshScaleGrid2D(state);
    }

  }
}
