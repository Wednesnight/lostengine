#include "lost/mesh/Mesh.h"
#include "lost/common/Logger.h"
#include "lost/math/Matrix.h"
#include "lost/gl/HybridVertexBuffer.h"
#include "lost/gl/HybridIndexBuffer.h"

namespace lost
{
namespace mesh
{

using namespace gl;

Mesh::Mesh()
{
  gl::BufferLayout layout;
  layout.add(gl::ET_vec3_f32, gl::UT_position, 0);
  layout.add(gl::ET_vec2_f32, gl::UT_texcoord0, 0);
  init(layout, gl::ET_u32);
}

Mesh::Mesh(const gl::BufferLayout& vertexLayout, gl::ElementType indexType)
{
  init(vertexLayout, indexType);
}

Mesh::~Mesh() 
{
//  DOUT("destroying mesh");
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


}
}

