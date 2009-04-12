#ifndef LOST_MESH_MESH
#define LOST_MESH_MESH

#include "lost/gl/ArrayBuffer.h"
#include "lost/gl/ElementArrayBuffer.h"
#include "lost/common/Logger.h"
#include "lost/gl/Context.h"
#include "lost/gl/State.h"
#include "lost/gl/StateParam.h"

namespace lost
{
namespace mesh
{

// Mesh base class, provides a render() method that applies the relevant minimal state 
// and issues the gl calls for drawing
struct Mesh
{
  virtual void draw(gl::ContextPtr ctx) = 0;
};

typedef boost::shared_ptr<Mesh> MeshPtr;

/** Mesh that stores it's data in GL buffer objects.
 */ 
template<
  typename IT,  // indextype
  typename VT,  // vertextype
  typename NT,  // normaltype
  typename CT,  // colortype
  typename TCT> // texcoordtype
struct BufferedMesh : public Mesh
{
  boost::shared_ptr<gl::ElementArrayBuffer<IT> >  indexBuffer; 
  boost::shared_ptr<gl::ArrayBuffer<VT> >         vertexBuffer;
  boost::shared_ptr<gl::ArrayBuffer<NT> >         normalBuffer;
  boost::shared_ptr<gl::ArrayBuffer<CT> >         colorBuffer;
  boost::shared_ptr<gl::ArrayBuffer<TCT> >        texCoordBuffer;
  
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

  gl::ParamPtr  vertexParam;
  gl::ParamPtr  normalParam;
  gl::ParamPtr  colorParam;
  gl::ParamPtr  texCoordParam;
  gl::StatePtr  meshState;

  GLenum drawMode; // GL_LINES, GL_TRIANGLES etc.
      
  BufferedMesh()
  {
  }
  
  virtual ~BufferedMesh()
  {
  }
  
  void indices(bool v) { if(v) indexBuffer.reset(new IndexBufferType); else indexBuffer.reset(); }
  void vertices(bool v) { if(v) vertexBuffer.reset(new VertexBufferType); else vertexBuffer.reset(); updateRenderState(); }
  void normals(bool v) { if(v) normalBuffer.reset(new NormalBufferType); else normalBuffer.reset(); updateRenderState();}
  void colors(bool v) { if(v) colorBuffer.reset(new ColorBufferType); else colorBuffer.reset(); updateRenderState(); }
  void texCoords(bool v) { if(v) texCoordBuffer.reset(new TexCoordBufferType); else texCoordBuffer.reset(); updateRenderState();}

  void updateRenderState()
  {
    vertexParam = gl::VertexArray::create(vertexBuffer ? true : false);
    normalParam = gl::NormalArray::create(normalBuffer ? true : false);
    colorParam = gl::ColorArray::create(colorBuffer ? true : false);
    texCoordParam = gl::TextureArray::create(texCoordBuffer ? true : false);
    meshState = gl::State::create(vertexParam, normalParam, colorParam, texCoordParam);
  }
  
  virtual void draw(gl::ContextPtr ctx)
  {
    ctx->pushState(meshState);
    vertexBuffer->bindVertexPointer();
    indexBuffer->bind();
    if(normalBuffer) normalBuffer->bindNormalPointer();
    if(colorBuffer) colorBuffer->bindColorPointer();
    if(texCoordBuffer) texCoordBuffer->bindTexCoordPointer();
    indexBuffer->drawElements(drawMode);
    if(texCoordBuffer) texCoordBuffer->unbind();
    if(colorBuffer) colorBuffer->unbind();
    if(normalBuffer) normalBuffer->unbind();
    indexBuffer->unbind();
    vertexBuffer->unbind();
    ctx->popState();
  }
  
};

typedef BufferedMesh<uint32_t, math::Vec3, math::Vec3, common::Color, math::Vec2> Mesh3D;
typedef boost::shared_ptr<Mesh3D> Mesh3DPtr;
typedef BufferedMesh<uint32_t, math::Vec2, math::Vec2, common::Color, math::Vec2> Mesh2D;
typedef boost::shared_ptr<Mesh2D> Mesh2DPtr;

}
}

#endif