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
      
      virtual gl::Buffer* getIndexBuffer() { return NULL; };
      virtual gl::Buffer* getVertexBuffer() { return NULL; };
      virtual gl::Buffer* getNormalBuffer() { return NULL; };
      virtual gl::Buffer* getColorBuffer() { return NULL; };
      virtual gl::Buffer* getTexCoordBuffer() { return NULL; };  
      
      GLenum drawMode; // GL_LINES, GL_TRIANGLES etc.
      MaterialPtr material;
      math::Matrix transform;
      gl::HybridVertexBufferPtr _vertexBuffer;
      gl::HybridIndexBufferPtr _indexBuffer;
    };

    typedef lost::shared_ptr<Mesh> MeshPtr;

    /** Mesh that stores it's data in GL buffer objects.
     *  A difficult question that comes up when dealing with buffers that are stored on
     * the gfx hardware is: what is the most efficient way to update that data?
     * Discussions on the net seem to suggest that these are the ways:
     *  * glBufferData: just replace all of the data associated with a buffer. The 
     *                  hardware will hopefully not block and keep using the old data
     *                  until the new render cycle is flushed.
     *  * glBufferSubData: similar to glBufferData, but since the memory region is reused
     *                  the gfx hardware might block until the memory area can be read and transferred
     *                  to the CPU.
     *  * glMap/UnmapBuffer: probably the worst of the three possibilities, since it probably blocks 
     *                  like solution two, but also requires more setup (at least two additional 
     *                  function calls for map/unmap).
     *  So, we'll treat this as a rule of thumb and manage meshes the following way:
     *  GL Buffers are mirrored in host memory. These data arrays are also the only areas that should
     *  be updated. Once all the updates were performed, the data can be sent to the hardware. 
     *
     *  Therefore, Meshes will always contain an <XXX>Buffer pointer, which is the way to access
     *  the buffer on the hardware, and a corresponding <XXX>Data pointer, which is the actual 
     *  data as mirrored on the host. Updates happen in the data locations and are then transferred
     *  to the hatfware buffers with the transferXXX helper functions.
     *
     * FIXME: is this really the best, even for interleaved arrays? Does this issues really matter
     * for plattforms like the iPhone, where data is generally a lot smaller. Also, on the desktop,
     * more and more functionality is moved to Shaders, so we don't have to transfer that much data
     * between host/gpu.
     */ 
    template<
      typename IT,  // indextype
      typename VT,  // vertextype
      typename NT,  // normaltype
      typename CT,  // colortype
      typename TCT> // texcoordtype
    struct BufferedMesh : public Mesh
    {
      // these are the actual hardware buffer objects, wrapped in our own helper classes, managed by shared ptrs
      lost::shared_ptr<gl::ElementArrayBuffer<IT> >  indexBuffer; 
      lost::shared_ptr<gl::ArrayBuffer<VT> >         vertexBuffer;
      lost::shared_ptr<gl::ArrayBuffer<NT> >         normalBuffer;
      lost::shared_ptr<gl::ArrayBuffer<CT> >         colorBuffer;
      lost::shared_ptr<gl::ArrayBuffer<TCT> >        texCoordBuffer;
      
      // this is the data contained in the buffers, mirrored in host/CPU RAM.
      // updates should happen here and be transferred to the GPU
      lost::shared_array<IT> indexData;
      lost::shared_array<VT> vertexData;
      lost::shared_array<NT> normalData;
      lost::shared_array<CT> colorData;
      lost::shared_array<TCT> texCoordData;
      
      // number of elements in host data arrays/buffer objects.
      // the type of the element might vary but will always be one of the template parameters.
      // these values are automatically set when you call the appropriate reset function.
      boost::uint32_t numIndices;
      boost::uint32_t numVertices;
      boost::uint32_t numNormals;
      boost::uint32_t numColors;
      boost::uint32_t numTexCoords;
      
      // use reset to create/destroy host data arrays
      // these functions also memorize the provided number of elements.
      // setting a size of 0 destroys the data array.
      // Note that the actual hardware buffer objects are NOT automatically 
      // created/destroyed if you use these functions.
      void resetIndices(boost::uint32_t n) { numIndices=n; if(n) {indexData.reset(new IT[n]);} else {indexData.reset();} _indexBuffer->reset(n);}
      void resetVertices(boost::uint32_t n) { numVertices=n; if(n) {vertexData.reset(new VT[n]);} else {vertexData.reset();} _vertexBuffer->reset(n);}
      void resetNormals(boost::uint32_t n) { numNormals=n; if(n) {normalData.reset(new NT[n]);} else {normalData.reset();}}
      void resetColors(boost::uint32_t n) { numColors=n; if(n) {colorData.reset(new CT[n]);} else {colorData.reset();}}
      void resetTexCoords(boost::uint32_t n) { numTexCoords=n; if(n) {texCoordData.reset(new TCT[n]);} else {texCoordData.reset();}}
      
      // use the following functions to transfer the data from the host to the gpu once you're done updating.
      // presence of a valid buffer object is NOT explicitly checked, so make sure the pointers are valid before calling.
      void transferIndices() { indexBuffer->bindBufferData(indexData.get(), numIndices); }
      void transferVertices() { vertexBuffer->bindBufferData(vertexData.get(), numVertices); }
      void transferNormals() { normalBuffer->bindBufferData(normalData.get(), numNormals); }
      void transferColors() { colorBuffer->bindBufferData(colorData.get(), numColors); }
      void transferTexCoords() { texCoordBuffer->bindBufferData(texCoordData.get(), numTexCoords); }
      
      // raw getters for the buffer objects.
      // these are convenience functions mostly intended for the gl::Context during draw operations.
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
      
      typedef gl::ElementArrayBuffer<IT>  IndexBufferType;
      typedef gl::ArrayBuffer<VT>         VertexBufferType;
      typedef gl::ArrayBuffer<NT>         NormalBufferType;
      typedef gl::ArrayBuffer<CT>         ColorBufferType;
      typedef gl::ArrayBuffer<TCT>        TexCoordBufferType;
          
      BufferedMesh()
      {
        // only simple scalar values need to be set here
        // since all smart pointer managed objects will be set to null in the constructors
        // of the smart pointers.
        numIndices = 0;
        numVertices = 0;
        numNormals = 0;
        numColors = 0;
        numTexCoords = 0;      
      }
      
      virtual ~BufferedMesh()
      {
      }
      
      // use these functions to create/destroy gpu buffer objects.
      // FIXME: for some reason the 'if(v && !buffer)' construct didn't work and buffers were always NULL, needs investigation
      void indices  (bool v) { if(v) indexBuffer.reset(new IndexBufferType); else indexBuffer.reset(); }
      void vertices (bool v) { if(v) vertexBuffer.reset(new VertexBufferType); else vertexBuffer.reset(); }
      void normals  (bool v) { if(v) normalBuffer.reset(new NormalBufferType); else normalBuffer.reset(); }
      void colors   (bool v) { if(v) colorBuffer.reset(new ColorBufferType); else colorBuffer.reset();  }
      void texCoords(bool v) { if(v) texCoordBuffer.reset(new TexCoordBufferType); else texCoordBuffer.reset(); }
      
    };

    // these are the typedefs you should use for easy instancing of a Mesh, because specifiying the full
    // parametrization can be tedious. Of course, you're not strictly bound to these variants, but they're 
    // sensible defaults for desktop applications. 
    // You might want to adjust index type and/or other types depending on the size of your data or the plattform
    // you're on. On the iPhone, buffer objects with indices in the 32bit range might not even be possible, and
    // 8 bit normal vectors feasible. Do your research.
    typedef BufferedMesh<uint32_t, math::Vec3, math::Vec3, common::Color, math::Vec2> Mesh3D;
    typedef lost::shared_ptr<Mesh3D> Mesh3DPtr;
    typedef BufferedMesh<uint32_t, math::Vec2, math::Vec2, common::Color, math::Vec2> Mesh2D;
    typedef lost::shared_ptr<Mesh2D> Mesh2DPtr;

  }
}

#endif
