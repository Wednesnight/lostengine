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

template<
  typename IT,  // indextype
  typename VT,  // vertextype
  typename NT,  // normaltype
  typename CT,  // colortype
  typename TCT> // texcoordtype
struct Mesh : public Primitive
{
  gl::ElementArrayBuffer<IT> indices; 
  gl::ArrayBuffer<VT>       vertices;
  gl::ArrayBuffer<NT>       normals;
  gl::ArrayBuffer<CT>        colors;
  gl::ArrayBuffer<TCT>     texcoords;
  
  MaterialPtr material; // texture, gl lighting material, shader with resources
  
  Mesh()
  {
  }
  
  virtual ~Mesh()
  {
  }
};
}
}

#endif