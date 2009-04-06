#ifndef LOST_SG_MESH
#define LOST_SG_MESH

#include "lost/sg/Primitive.h"
#include "lost/sg/Material.h"
#include "lost/gl/ArrayBuffer.h"
#include "lost/gl/ElementArrayBuffer.h"

namespace lost
{
namespace sg
{

/** Basic Mesh type for usage in scene graph.
 * Basic usage: 
 * * initialize size once in constructor
 * * create/Delete feature arrays as required (color, texcoord etc.)
 * * shadow data by keeping a copy in CPU RAM, sending it as an update to the buffer if required
 * * for small amounts of data, send updates to buffers using bufferSubData
 * * for large amounts, use mapBufferData, but watch the performance, as bufferSubData might still be faster, even for large updates
 * 
 */ 
template<
  typename IT,  // indextype
  typename VT,  // vertextype
  typename NT,  // normaltype
  typename CT,  // colortype
  typename TCT> // texcoordtype
struct Mesh : public Primitive
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
  
  MaterialPtr material; // texture, gl lighting material, shader with resources
  
  common::Color color; // this is the global color that will be set before drawing
  
  Mesh()
  {
  }
  
  virtual ~Mesh()
  {
  }
};

typedef Mesh<uint32_t, math::Vec3, math::Vec2, common::Color, math::Vec2> Mesh3D;
typedef Mesh<uint32_t, math::Vec2, math::Vec2, common::Color, math::Vec2> Mesh2D;

}
}

#endif