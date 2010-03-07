#ifndef LOST_MESH_MESH
#define LOST_MESH_MESH

#include "lost/mesh/Material.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec4.h"
#include "lost/math/Matrix.h"
#include "lost/gl/BufferLayout.h"

namespace lost
{

namespace gl
{
  struct HybridBuffer;
  typedef lost::shared_ptr<HybridBuffer> HybridBufferPtr;

  struct HybridVertexBuffer;
  typedef lost::shared_ptr<HybridVertexBuffer> HybridVertexBufferPtr;

  struct HybridIndexBuffer;
  typedef lost::shared_ptr<HybridIndexBuffer> HybridIndexBufferPtr;
}

namespace mesh
{

struct Material;
typedef lost::shared_ptr<Material> MaterialPtr;

struct Mesh;
typedef lost::shared_ptr<Mesh> MeshPtr;

// Mesh base class, provides a render() method that applies the relevant minimal state 
// and issues the gl calls for drawing
struct Mesh
{
  Mesh();
  Mesh(const gl::BufferLayout& vertexLayout, gl::ElementType indexType);
  // completely initializes the mesh object with default values. This is usually called from a constructor
  void init(const gl::BufferLayout& vertexLayout, gl::ElementType indexType);
  virtual ~Mesh();
  
  static MeshPtr create() { return MeshPtr(new Mesh); }
  static MeshPtr create(const gl::BufferLayout& vertexLayout, gl::ElementType indexType) { return MeshPtr(new Mesh(vertexLayout, indexType)); }
  
  // resets the buffer types, throwing away all previously stored/allocated data. you need to call resetSize after this one.
  void resetBuffers(const gl::BufferLayout& vertexLayout, gl::ElementType indexType);

  // resets the size of vertex and index buffers
  void resetSize(uint32_t numVertices, uint32_t numIndices);

  // helper functions that could easily be replaced by direct access of buffers but were needed for migration
  // FIXME: these are here for legacy reasons, should really be replaced with the versions below
  void setIndex(uint32_t idx, uint32_t val);
  void setVertex(uint32_t idx, const math::Vec2& val);
  void setTexCoord(uint32_t idx, const math::Vec2& val);
  math::Vec2 getVertex(uint32_t idx);

  gl::HybridBufferPtr bufferFromUsageType(gl::UsageType ut);

  void set(uint32_t idx, gl::UsageType ut, uint8_t val);
  void set(uint32_t idx, gl::UsageType ut, uint16_t val);
  void set(uint32_t idx, gl::UsageType ut, uint32_t val);
  void set(uint32_t idx, gl::UsageType ut, float val);
  void set(uint32_t idx, gl::UsageType ut, const math::Vec2& val);
  void set(uint32_t idx, gl::UsageType ut, const math::Vec3& val);
  void set(uint32_t idx, gl::UsageType ut, const math::Vec4& val);
  void set(uint32_t idx, gl::UsageType ut, const common::Color& val);
  
  math::Vec2 getAsVec2(uint32_t idx, gl::UsageType ut);
  math::Vec3 getAsVec3(uint32_t idx, gl::UsageType ut);
  uint32_t   getAsU32(uint32_t idx, gl::UsageType ut);  


  GLenum drawMode; // GL_LINES, GL_TRIANGLES etc.
  MaterialPtr material;
  math::Matrix transform;
  gl::HybridVertexBufferPtr vertexBuffer;
  gl::HybridIndexBufferPtr indexBuffer;
};

}
}

#endif
