#include "lost/lua/bindings/LostMesh.h"
#include "lost/lua/lua.h"

#include "lost/mesh/Line.h"
#include "lost/mesh/Loader.h"
#include "lost/mesh/Material.h"
#include "lost/mesh/Mesh.h"
#include "lost/mesh/Quad.h"
#include "lost/mesh/Disc.h"
#include "lost/mesh/ScaleGrid.h"
#include "lost/mesh/Rect.h"
#include "lost/mesh/Circular.h"
#include "lost/platform/shared_ptr.h"
#include "lost/gl/Texture.h"
#include "lost/gl/ShaderProgram.h"
#include "lost/gl/UniformBlock.h"
#include "lost/mesh/TextureManager.h"
#include "lost/mesh/RoundedRect.h"
#include "lost/mesh/Sphere.h"
#include "lost/gl/HybridVertexBuffer.h"
#include "lost/gl/HybridIndexBuffer.h"

using namespace luabind;
using namespace lost::mesh;

namespace lost
{
  namespace lua
  {
/*    void LostMeshLine(lua_State* state)
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
    }*/

    void LostMeshRect(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<Rect, Mesh>("Rect")
            .def("updateSize", &mesh::Rect::updateSize)
            .scope
            [
              def("create", &mesh::Rect::create)
            ]
        ]
      ];
    }
    
/*    void LostMeshCircular(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<Circle2D, Mesh>("Circle2D")
            .scope
            [
              def("create", &Circle2D::create)
            ],
          class_<FilledCircle2D, Mesh>("FilledCircle2D")
            .scope
            [
              def("create", &FilledCircle2D::create)
            ],
          class_<Ellipse2D, Mesh>("Ellipse2D")
            .scope
            [
              def("create", &Ellipse2D::create)
            ],
          class_<FilledEllipse2D, Mesh>("FilledEllipse2D")
            .scope
            [
              def("create", &FilledEllipse2D::create)
            ]
        ]
      ];
    }*/
    
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

    void LostMeshMaterial_addTexture(Material* material, const gl::TexturePtr& texture)
    {
      material->textures.push_back(texture);
    }

    void LostMeshMaterial(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<Material>("Material")
            .def("addTexture", &LostMeshMaterial_addTexture)
            .def_readwrite("shader", &Material::shader)
            .def_readwrite("uniforms", &Material::uniforms)
            .def_readwrite("color", &Material::color)
            .def_readwrite("blend", &Material::blend)
            .def_readwrite("blendSrc", &Material::blendSrc)
            .def_readwrite("blendDest", &Material::blendDest)
            .def_readwrite("cull", &Material::cull)
            .def_readwrite("cullMode", &Material::cullMode)
            .def("add", &Material::add)
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
            .def_readwrite("vertexBuffer", &Mesh::vertexBuffer)
            .def_readwrite("indexBuffer", &Mesh::indexBuffer)
            .def_readwrite("material", &Mesh::material)
            .def_readwrite("transform", &Mesh::transform)
            .def_readwrite("drawMode", &Mesh::drawMode)
            .def("resetBuffers", &Mesh::resetBuffers)
            .def("resetSize", &Mesh::resetSize)
            .def("setU8", (void(Mesh::*)(uint32_t, gl::UsageType, uint8_t))&Mesh::set)
            .def("setU16", (void(Mesh::*)(uint32_t idx, gl::UsageType ut, uint16_t val))&Mesh::set)
            .def("setU32", (void(Mesh::*)(uint32_t idx, gl::UsageType ut, uint32_t val))&Mesh::set)
            .def("setF32", (void(Mesh::*)(uint32_t idx, gl::UsageType ut, float val))&Mesh::set)
            .def("set", (void(Mesh::*)(uint32_t idx, gl::UsageType ut, const math::Vec2& val))&Mesh::set)
            .def("set", (void(Mesh::*)(uint32_t idx, gl::UsageType ut, const math::Vec3& val))&Mesh::set)
            .def("set", (void(Mesh::*)(uint32_t idx, gl::UsageType ut, const math::Vec4& val))&Mesh::set)
            .def("set", (void(Mesh::*)(uint32_t idx, gl::UsageType ut, const common::Color& val))&Mesh::set)
            .def("getAsVec2",&Mesh::getAsVec2)
            .def("getAsVec3",&Mesh::getAsVec3)
            .def("getAsU32",&Mesh::getAsU32)
            .scope
            [
              def("create", (MeshPtr(*)())&Mesh::create),
              def("create", (MeshPtr(*)(const gl::BufferLayout& vertexLayout, gl::ElementType indexType))&Mesh::create)
            ]
        ]
      ];
    }

    void LostMeshQuad(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<Quad, Mesh>("Quad")
            .def("updateSize", &Quad::updateSize)
            .scope
            [
              def("create", (QuadPtr(*)()) &Quad::create),
              def("create", (QuadPtr(*)(const math::Rect& inRect)) &Quad::create),
              def("create", (QuadPtr(*)(common::DataPtr data, bool)) &Quad::create),
              def("create", (QuadPtr(*)(gl::TexturePtr tex, bool)) &Quad::create)
            ]
        ]
      ];
    }

    void LostMeshDisc(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<Disc, Mesh>("Disc")
            .def("update", &Disc::update)
            .def("updateRadius", &Disc::updateRadius)
            .def("updateLineWidth", &Disc::updateLineWidth)
            .scope
            [
              def("create", &Disc::create)
            ]
        ]
      ];
    }

    void LostMeshSphere(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<Sphere, Mesh>("Sphere")
            .def("updateRadius", &Sphere::updateRadius)
            .def("updateSubdivisions", &Sphere::updateSubdivisions)
            .scope
            [
              def("create", &Sphere::create)
            ]
        ]
      ];
    }
    
    void LostMeshScaleGrid(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<ScaleGrid, Mesh>("ScaleGrid")
          .def("updateSize", &ScaleGrid::updateSize)
          .scope
          [
            def("create", &ScaleGrid::create)            
          ]
        ]
      ];
    }

    void LostMeshRoundedRect(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<RoundedRect, Mesh>("RoundedRect")
          .def("size", &RoundedRect::size)
          .def("roundCorners", &RoundedRect::roundCorners)
          .def("radius", &RoundedRect::radius)
          .def("showSides", &RoundedRect::showSides)
          .scope
          [
            def("create", &RoundedRect::create)            
          ]
        ]
      ];
    }

    void LostMeshTextureManager(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<TextureManager>("TextureManager")
          .def("arcTexture", &TextureManager::arcTexture)
          .def("arcFilledTexture", &TextureManager::arcFilledTexture)
          .def("addGradient", &TextureManager::addGradient)
          .def_readonly("gradientTexture", &TextureManager::gradientTexture)
          .def_readwrite("gradientTextureWidth", &TextureManager::gradientTextureWidth)
          .scope
          [
            def("create", (TextureManagerPtr(*)())&TextureManager::create)            
          ]
        ]
      ];      
    }


    void LostMesh(lua_State* state)
    {
      // mesh first because following classes are based on it
      LostMeshMesh(state);
//      LostMeshLine(state);
      LostMeshRect(state);
//      LostMeshCircular(state);
      LostMeshLoader(state);
      LostMeshMaterial(state);
      LostMeshQuad(state);
      LostMeshDisc(state);
      LostMeshSphere(state);
      LostMeshScaleGrid(state);
      LostMeshRoundedRect(state);      
      LostMeshTextureManager(state);
    }

  }
}
