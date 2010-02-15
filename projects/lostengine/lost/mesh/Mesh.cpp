#include "lost/mesh/Mesh.h"
#include "lost/common/Logger.h"

namespace lost
{
namespace mesh
{

Mesh::Mesh()
{
  gl::BufferLayout layout;
  layout.add(gl::ET_vec3_f32, gl::UT_vertex, 0);
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

void Mesh::setIndex(uint32_t idx, uint32_t val)
{
  indexBuffer->set(idx, gl::UT_index, val);
}

void Mesh::setVertex(uint32_t idx, const math::Vec2& val)
{
  vertexBuffer->set(idx, gl::UT_vertex, val);
}

void Mesh::setTexCoord(uint32_t idx, const math::Vec2& val)
{
  vertexBuffer->set(idx, gl::UT_texcoord0, val);
}

math::Vec2 Mesh::getVertex(uint32_t idx)
{
  return vertexBuffer->getAsVec2(idx, gl::UT_vertex);
}

}
}

