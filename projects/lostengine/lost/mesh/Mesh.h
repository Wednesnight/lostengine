#ifndef LOST_MESH_MESH
#define LOST_MESH_MESH

#include "lost/gl/ArrayBuffer.h"
#include "lost/gl/ElementArrayBuffer.h"
#include "lost/common/Logger.h"
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
    // Mesh base class, provides a render() method that applies the relevant minimal state 
    // and issues the gl calls for drawing
    struct Mesh
    {

      void resetBuffers(const gl::BufferLayout& vertexLayout, gl::ElementType indexType)
      {
        _vertexBuffer.reset(new gl::HybridVertexBuffer(vertexLayout));
        _indexBuffer.reset(new gl::HybridIndexBuffer(indexType));
      }

      Mesh()
      {
        material = Material::create();
        transform.initIdentity();
        gl::BufferLayout layout;
        layout.add(gl::ET_vec3_f32, gl::UT_vertex, 0);
        layout.add(gl::ET_vec2_f32, gl::UT_texcoord0, 0);
        resetBuffers(layout, gl::ET_u32);
//        DOUT("creating mesh");
      }

      void setIndex(uint32_t idx, uint32_t val)
      {
        _indexBuffer->set(idx, gl::UT_index, val);
      }
      
      void setVertex(uint32_t idx, const math::Vec2& val)
      {
        _vertexBuffer->set(idx, gl::UT_vertex, val);
      }
      
      void setTexCoord(uint32_t idx, const math::Vec2& val)
      {
        _vertexBuffer->set(idx, gl::UT_texcoord0, val);
      }
      
      math::Vec2 getVertex(uint32_t idx)
      {
        return _vertexBuffer->getAsVec2(idx, gl::UT_vertex);
      }

      virtual ~Mesh() { /*DOUT("destroying mesh");*/ }
            
      GLenum drawMode; // GL_LINES, GL_TRIANGLES etc.
      MaterialPtr material;
      math::Matrix transform;
      gl::HybridVertexBufferPtr _vertexBuffer;
      gl::HybridIndexBufferPtr _indexBuffer;
    };

    typedef lost::shared_ptr<Mesh> MeshPtr;
  }
}

#endif
