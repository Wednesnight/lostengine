#ifndef LOST_SG_MESH
#define LOST_SG_MESH

#include <boost/shared_ptr.hpp>
#include "lost/gl/ArrayBuffer.h"
#include "lost/gl/ElementArrayBuffer.h"
#include "lost/common/Logger.h"
#include "lost/gl/Context.h"

namespace lost
{
namespace sg
{

// Mesh base class, provides a render() method that applies the relevant minimal state 
// and issues the gl calls for drawing
struct Mesh
{
  virtual void render(gl::ContextPtr ctx) = 0;
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
  boost::shared_ptr<gl::ElementArrayBuffer<IT> >  indices; 
  boost::shared_ptr<gl::ArrayBuffer<VT> >         vertices;
  boost::shared_ptr<gl::ArrayBuffer<NT> >         normals;
  boost::shared_ptr<gl::ArrayBuffer<CT> >         colors;
  boost::shared_ptr<gl::ArrayBuffer<TCT> >        texcoords;
  
  typedef IT IndexType;
  typedef VT VertexType;
  typedef NT NormalType;
  typedef CT ColorType;
  typedef TCT TexCoordType;
      
  BufferedMesh()
  {
  }
  
  virtual ~BufferedMesh()
  {
  }
  
  virtual void render(gl::ContextPtr ctx)
  {
    DOUT("render mesh");
  }
  
};

typedef BufferedMesh<uint32_t, math::Vec3, math::Vec2, common::Color, math::Vec2> Mesh3D;
typedef BufferedMesh<uint32_t, math::Vec2, math::Vec2, common::Color, math::Vec2> Mesh2D;

}
}

#endif