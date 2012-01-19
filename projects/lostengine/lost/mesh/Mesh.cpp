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

#include "lost/mesh/Mesh.h"
#include "lost/common/Logger.h"
#include "lost/math/Matrix.h"
#include "lost/gl/HybridVertexBuffer.h"
#include "lost/gl/HybridIndexBuffer.h"
#include "lost/profiler/Blackbox.h"

namespace lost
{
namespace mesh
{

using namespace gl;

#define bb_mesh_key "lost.mesh.Mesh"
Mesh::Mesh()
{
  gl::BufferLayout layout;
  layout.add(gl::ET_vec3_f32, gl::UT_position, 0);
  layout.add(gl::ET_vec2_f32, gl::UT_texcoord0, 0);
  init(layout, gl::ET_u16);
  BB_INC(bb_mesh_key);
}

Mesh::Mesh(const gl::BufferLayout& vertexLayout, gl::ElementType indexType)
{
  init(vertexLayout, indexType);
  BB_INC(bb_mesh_key);
}

Mesh::~Mesh() 
{
//  DOUT("destroying mesh");
  BB_DEC(bb_mesh_key);
}

void Mesh::init(const gl::BufferLayout& vertexLayout, gl::ElementType indexType)
{
//  DOUT("creating mesh");
  material = Material::create();
  transform.initIdentity();
  resetBuffers(vertexLayout, indexType);
}

void Mesh::resetBuffers(const gl::BufferLayout& vertexLayout, gl::ElementType indexType)
{
  vertexBuffer.reset(new gl::HybridVertexBuffer(vertexLayout));
  indexBuffer.reset(new gl::HybridIndexBuffer(indexType));
}

void Mesh::resetSize(uint32_t numVertices, uint32_t numIndices)
{
  vertexBuffer->reset(numVertices);
  indexBuffer->reset(numIndices);
}

HybridBufferPtr Mesh::bufferFromUsageType(UsageType ut)
{
  if(ut == UT_index)
    return indexBuffer;
  else
    return vertexBuffer;
}

void Mesh::setIndex(uint32_t idx, uint32_t val)
{
  indexBuffer->set(idx, gl::UT_index, val);
}

void Mesh::setVertex(uint32_t idx, const math::Vec2& val)
{
  vertexBuffer->set(idx, gl::UT_position, val);
}

void Mesh::setTexCoord(uint32_t idx, const math::Vec2& val)
{
  vertexBuffer->set(idx, gl::UT_texcoord0, val);
}

math::Vec2 Mesh::getVertex(uint32_t idx)
{
  return vertexBuffer->getAsVec2(idx, gl::UT_position);
}

void Mesh::set(uint32_t idx, UsageType ut, uint8_t val) { bufferFromUsageType(ut)->set(idx, ut, val); }
void Mesh::set(uint32_t idx, UsageType ut, uint16_t val) { bufferFromUsageType(ut)->set(idx, ut, val); }
void Mesh::set(uint32_t idx, UsageType ut, uint32_t val) { bufferFromUsageType(ut)->set(idx, ut, val); }
void Mesh::set(uint32_t idx, UsageType ut, float val) { bufferFromUsageType(ut)->set(idx, ut, val); }
void Mesh::set(uint32_t idx, UsageType ut, const math::Vec2& val) { bufferFromUsageType(ut)->set(idx, ut, val); }
void Mesh::set(uint32_t idx, UsageType ut, const math::Vec3& val) { bufferFromUsageType(ut)->set(idx, ut, val); }
void Mesh::set(uint32_t idx, UsageType ut, const math::Vec4& val) { bufferFromUsageType(ut)->set(idx, ut, val); }
void Mesh::set(uint32_t idx, UsageType ut, const common::Color& val) { bufferFromUsageType(ut)->set(idx, ut, val); }

math::Vec2 Mesh::getAsVec2(uint32_t idx, UsageType ut) { return bufferFromUsageType(ut)->getAsVec2(idx, ut); }
math::Vec3 Mesh::getAsVec3(uint32_t idx, UsageType ut) { return bufferFromUsageType(ut)->getAsVec3(idx, ut); }
uint32_t   Mesh::getAsU32(uint32_t idx, UsageType ut) { return bufferFromUsageType(ut)->getAsU32(idx, ut); }

MeshPtr Mesh::clone()
{
  MeshPtr result(new Mesh);
  
  result->material = material;
  result->transform = transform;
  result->vertexBuffer = vertexBuffer;
  result->indexBuffer = indexBuffer;
  
  return result;
}

uint32_t Mesh::numVertices()
{
  return vertexBuffer->hostBuffer->count; 
}

}
}

