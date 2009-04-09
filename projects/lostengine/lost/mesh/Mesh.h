#ifndef LOST_MESH_MESH
#define LOST_MESH_MESH

#include <boost/shared_ptr.hpp>
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

  gl::StateParamPtr  vertexParam;
  gl::StateParamPtr  normalParam;
  gl::StateParamPtr  colorParam;
  gl::StateParamPtr  texCoordParam;
  gl::StatePtr  meshState;

  GLenum drawMode; // GL_LINES, GL_TRIANGLES etc.
      
  BufferedMesh()
  {
    vertexParam = gl::VertexArray::create(false);
    normalParam = gl::NormalArray::create(false);
    colorParam = gl::ColorArray::create(false);
    texCoordParam = gl::TextureArray::create(false);
    meshState = gl::State::create(vertexParam, normalParam, colorParam, texCoordParam);
  }
  
  virtual ~BufferedMesh()
  {
  }
  
  void indices(bool v) { if(v) indexBuffer.reset(new IndexBufferType); else indexBuffer.reset(); }
  void vertices(bool v) { if(v) vertexBuffer.reset(new VertexBufferType); else vertexBuffer.reset(); vertexParam->enable=v; }
  void normals(bool v) { if(v) normalBuffer.reset(new NormalBufferType); else normalBuffer.reset(); normalParam->enable=v;}
  void colors(bool v) { if(v) colorBuffer.reset(new ColorBufferType); else colorBuffer.reset(); colorParam->enable=v; }
  void texCoords(bool v) { if(v) texCoordBuffer.reset(new TexCoordBufferType); else texCoordBuffer.reset(); texCoordParam->enable=v;}
  
  virtual void draw(gl::ContextPtr ctx)
  {
    ctx->pushState(meshState);
    vertexBuffer->bindVertexPointer();
    indexBuffer->bind();
    if(normalBuffer) normalBuffer->bindNormalPointer();
    if(colorBuffer) colorBuffer->bindColorPointer();
    if(texCoordBuffer) texCoordBuffer->bindTexCoordPointer();
    indexBuffer->drawElements(drawMode);
    ctx->popState();
  }
  
};

typedef BufferedMesh<uint32_t, math::Vec3, math::Vec2, common::Color, math::Vec2> Mesh3D;
typedef BufferedMesh<uint32_t, math::Vec2, math::Vec2, common::Color, math::Vec2> Mesh2D;

}
}

#endif