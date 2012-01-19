/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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
#include "lost/common/ColorGradient.h"

#include <slub/slub.h>

using namespace lost::mesh;

namespace lost
{
  namespace lua
  {

    void LostMeshLine(lua_State* state)
    {
      slub::package(state, "lost").package("mesh")
        .clazz<Line>("Line")
          .extends<Mesh>()
          .function("create", &Line::create);
    }

    void LostMeshRect(lua_State* state)
    {
      slub::package(state, "lost").package("mesh")
        .clazz<Rect>("Rect")
          .extends<Mesh>()
          .method("updateSize", &mesh::Rect::updateSize)
          .function("create", &mesh::Rect::create);
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
      slub::package(state, "lost").package("mesh")
        .clazz<Loader>("Loader")
          .function("obj", &lost::mesh::Loader::obj);
    }

    void LostMeshMaterial_addTexture(Material* material, const gl::TexturePtr& texture)
    {
      material->textures.push_back(texture);
    }

    void LostMeshMaterial(lua_State* state)
    {
      slub::package(state, "lost").package("mesh")
        .clazz<Material>("Material")
          .method("addTexture", &LostMeshMaterial_addTexture)
          .field("shader", &Material::shader)
          .field("uniforms", &Material::uniforms)
          .field("color", &Material::color)
          .field("blend", &Material::blend)
          .field("blendSrc", &Material::blendSrc)
          .field("blendDest", &Material::blendDest)
          .field("cull", &Material::cull)
          .field("cullMode", &Material::cullMode)
          .method("add", &Material::add)
          .method("setTexture", &Material::setTexture)
          .method("getTexture", &Material::getTexture)
          .method("limitTextures", &Material::limitTextures)
          .method("blendNormal", &Material::blendNormal)
          .method("blendPremultiplied", &Material::blendPremultiplied)
          .method("blendOff", &Material::blendOff)
          .method("clone", &Material::clone)
          .function("create", (MaterialPtr(*)())&Material::create);
    }

    void LostMeshMesh(lua_State* state)
    {
      slub::package(state, "lost").package("mesh")
        .clazz<Mesh>("Mesh")
          .field("vertexBuffer", &Mesh::vertexBuffer)
          .field("indexBuffer", &Mesh::indexBuffer)
          .field("material", &Mesh::material)
          .field("transform", &Mesh::transform)
          .method("resetBuffers", &Mesh::resetBuffers)
          .method("resetSize", &Mesh::resetSize)
          .method("setU8", (void(Mesh::*)(uint32_t, gl::UsageType, uint8_t))&Mesh::set)
          .method("setU16", (void(Mesh::*)(uint32_t idx, gl::UsageType ut, uint16_t val))&Mesh::set)
          .method("setU32", (void(Mesh::*)(uint32_t idx, gl::UsageType ut, uint32_t val))&Mesh::set)
          .method("setF32", (void(Mesh::*)(uint32_t idx, gl::UsageType ut, float val))&Mesh::set)
          .method("set", (void(Mesh::*)(uint32_t idx, gl::UsageType ut, const math::Vec2& val))&Mesh::set)
          .method("set", (void(Mesh::*)(uint32_t idx, gl::UsageType ut, const math::Vec3& val))&Mesh::set)
          .method("set", (void(Mesh::*)(uint32_t idx, gl::UsageType ut, const math::Vec4& val))&Mesh::set)
          .method("set", (void(Mesh::*)(uint32_t idx, gl::UsageType ut, const common::Color& val))&Mesh::set)
          .method("getAsVec2",&Mesh::getAsVec2)
          .method("getAsVec3",&Mesh::getAsVec3)
          .method("getAsU32",&Mesh::getAsU32)
          .method("clone", &Mesh::clone)
          .function("create", (MeshPtr(*)())&Mesh::create)
          .function("create", (MeshPtr(*)(const gl::BufferLayout& vertexLayout, gl::ElementType indexType))&Mesh::create);
    }

    void LostMeshQuad(lua_State* state)
    {
      slub::package(state, "lost").package("mesh")
        .clazz<Quad>("Quad")
          .extends<Mesh>()
          .method("updateSize", &Quad::updateSize)
          .function("create", (QuadPtr(*)()) &Quad::create)
          .function("create", (QuadPtr(*)(const math::Rect& inRect)) &Quad::create)
          .function("create", (QuadPtr(*)(common::DataPtr data, bool)) &Quad::create)
          .function("create", (QuadPtr(*)(gl::TexturePtr tex, bool)) &Quad::create);
    }

    void LostMeshDisc(lua_State* state)
    {
      slub::package(state, "lost").package("mesh")
        .clazz<Disc>("Disc")
          .extends<Mesh>()
          .method("update", &Disc::update)
          .method("updateRadius", &Disc::updateRadius)
          .method("updateLineWidth", &Disc::updateLineWidth)
          .function("create", &Disc::create);
    }

    void LostMeshSphere(lua_State* state)
    {
      slub::package(state, "lost").package("mesh")
        .clazz<Sphere>("Sphere")
          .extends<Mesh>()
          .method("updateRadius", &Sphere::updateRadius)
          .method("updateSubdivisions", &Sphere::updateSubdivisions)
          .function("create", &Sphere::create);
    }
    
    void LostMeshScaleGrid(lua_State* state)
    {
      slub::package(state, "lost").package("mesh")
        .clazz<ScaleGrid>("ScaleGrid")
          .extends<Mesh>()
          .method("updateSize", &ScaleGrid::updateSize)
          .function("create", &ScaleGrid::create);
    }

    void LostMeshRoundedRect(lua_State* state)
    {
      slub::package(state, "lost").package("mesh")
        .clazz<RoundedRect>("RoundedRect")
          .extends<Mesh>()
          .method("size", &RoundedRect::size)
          .method("roundCorners", &RoundedRect::roundCorners)
          .method("radius", &RoundedRect::radius)
          .method("showSides", &RoundedRect::showSides)
          .method("lineWidth", &RoundedRect::lineWidth)
          .function("create", &RoundedRect::create);
    }

    void LostMeshTextureManager(lua_State* state)
    {
      slub::package(state, "lost").package("mesh")
        .clazz<TextureManager>("TextureManager")
          .method("arcTexture", &TextureManager::arcTexture)
          .method("arcFilledTexture", &TextureManager::arcFilledTexture)
          .method("addGradient", &TextureManager::addGradient)
          .field("gradientTexture", &TextureManager::gradientTexture)
          .field("gradientTextureWidth", &TextureManager::gradientTextureWidth)
          .function("create", (TextureManagerPtr(*)())&TextureManager::create);
    }


    void LostMesh(lua_State* state)
    {
      // mesh first because following classes are based on it
      LostMeshMesh(state);
      LostMeshLine(state);
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
