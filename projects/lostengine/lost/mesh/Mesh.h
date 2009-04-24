#ifndef LOST_MESH_MESH
#define LOST_MESH_MESH

#include "lost/gl/ArrayBuffer.h"
#include "lost/gl/ElementArrayBuffer.h"
#include "lost/common/Logger.h"
#include "lost/mesh/Material.h"
#include "lost/math/Matrix.h"
#include "lost/math/Vec2.h"

namespace lost
{
  namespace mesh
  {
    // used for piexl perfect 2D positioning
    // this one does nothing since Vec2 constructor defaults to 0/0
    struct VertexOffsetVectorZero
    {
      static inline float x() { return 0.0f; }
      static inline float y() { return 0.0f; }
    };
  
    // this one moves pixels by .5 to center a logical pixel on the physical one.
    struct VertexOffsetVector2D 
    {
      static inline float x() { return .5f; }
      static inline float y() { return .5f; }      
    };

    // Mesh base class, provides a render() method that applies the relevant minimal state 
    // and issues the gl calls for drawing
    struct Mesh
    {

      Mesh()
      {
        material = Material::create();
        modelTransform.initIdentity();
      }

      virtual ~Mesh() {}
      
      virtual gl::Buffer* getIndexBuffer() { return NULL; };
      virtual gl::Buffer* getVertexBuffer() { return NULL; };
      virtual gl::Buffer* getNormalBuffer() { return NULL; };
      virtual gl::Buffer* getColorBuffer() { return NULL; };
      virtual gl::Buffer* getTexCoordBuffer() { return NULL; };  
      
      GLenum drawMode; // GL_LINES, GL_TRIANGLES etc.
      MaterialPtr material;
      math::Matrix modelTransform;
    };

    typedef lost::shared_ptr<Mesh> MeshPtr;

    /** Mesh that stores it's data in GL buffer objects.
     */ 
    template<
      typename IT,  // indextype
      typename VT,  // vertextype
      typename NT,  // normaltype
      typename CT,  // colortype
      typename TCT, // texcoordtype
      typename OFFV> // vertex offset vector for 2D pixex corrections
    struct BufferedMesh : public Mesh
    {
      lost::shared_ptr<gl::ElementArrayBuffer<IT> >  indexBuffer; 
      lost::shared_ptr<gl::ArrayBuffer<VT> >         vertexBuffer;
      lost::shared_ptr<gl::ArrayBuffer<NT> >         normalBuffer;
      lost::shared_ptr<gl::ArrayBuffer<CT> >         colorBuffer;
      lost::shared_ptr<gl::ArrayBuffer<TCT> >        texCoordBuffer;
      
      gl::Buffer* getIndexBuffer() { return indexBuffer.get(); }
      gl::Buffer* getVertexBuffer() { return vertexBuffer.get(); };
      gl::Buffer* getNormalBuffer() { return normalBuffer.get(); };
      gl::Buffer* getColorBuffer() { return colorBuffer.get(); };
      gl::Buffer* getTexCoordBuffer() {return texCoordBuffer.get(); };  
      
      typedef IT IndexType;
      typedef VT VertexType;
      typedef NT NormalType;
      typedef CT ColorType;
      typedef TCT TexCoordType;
      typedef OFFV OffsetVectorType;
      
      typedef gl::ElementArrayBuffer<IT>  IndexBufferType;
      typedef gl::ArrayBuffer<VT>         VertexBufferType;
      typedef gl::ArrayBuffer<NT>         NormalBufferType;
      typedef gl::ArrayBuffer<CT>         ColorBufferType;
      typedef gl::ArrayBuffer<TCT>        TexCoordBufferType;
          
      BufferedMesh()
      {
      }
      
      virtual ~BufferedMesh()
      {
      }
      
      void indices  (bool v) { if(v) indexBuffer.reset(new IndexBufferType); else indexBuffer.reset(); }
      void vertices (bool v) { if(v) vertexBuffer.reset(new VertexBufferType); else vertexBuffer.reset(); }
      void normals  (bool v) { if(v) normalBuffer.reset(new NormalBufferType); else normalBuffer.reset(); }
      void colors   (bool v) { if(v) colorBuffer.reset(new ColorBufferType); else colorBuffer.reset();  }
      void texCoords(bool v) { if(v) texCoordBuffer.reset(new TexCoordBufferType); else texCoordBuffer.reset(); }
      
    };

    typedef BufferedMesh<uint32_t, math::Vec3, math::Vec3, common::Color, math::Vec2, VertexOffsetVectorZero> Mesh3D;
    typedef lost::shared_ptr<Mesh3D> Mesh3DPtr;
    typedef BufferedMesh<uint32_t, math::Vec2, math::Vec2, common::Color, math::Vec2, VertexOffsetVector2D> Mesh2D;
    typedef lost::shared_ptr<Mesh2D> Mesh2DPtr;

  }
}

#endif
