#ifndef LOST_MESH_MESH
#define LOST_MESH_MESH

#include "lost/mesh/Material.h"
#include "lost/math/Matrix.h"
#include "lost/math/Vec2.h"
#include "lost/gl/BufferLayout.h"
#include "lost/gl/HybridVertexBuffer.h"
#include "lost/gl/HybridIndexBuffer.h"

namespace lost
{
namespace mesh
{

struct Mesh;
typedef lost::shared_ptr<Mesh> MeshPtr;

// Mesh base class, provides a render() method that applies the relevant minimal state 
// and issues the gl calls for drawing
struct Mesh
{
  Mesh();
  virtual ~Mesh();
  void resetBuffers(const gl::BufferLayout& vertexLayout, gl::ElementType indexType);

  // helper functions that could easily be replaced by direct access of buffers but were needed for migration
  void setIndex(uint32_t idx, uint32_t val);
  void setVertex(uint32_t idx, const math::Vec2& val);
  void setTexCoord(uint32_t idx, const math::Vec2& val);
  math::Vec2 getVertex(uint32_t idx);

  GLenum drawMode; // GL_LINES, GL_TRIANGLES etc.
  MaterialPtr material;
  math::Matrix transform;
  gl::HybridVertexBufferPtr vertexBuffer;
  gl::HybridIndexBufferPtr indexBuffer;
};

}
}

#endif
