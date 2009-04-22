#ifndef LOST_MESH_LINE
#define LOST_MESH_LINE

#include "lost/mesh/Mesh.h"

namespace lost
{
namespace mesh
{

template<typename MESHTYPE>
struct Line : public MESHTYPE
{
  typedef typename MESHTYPE::VertexType VertexType;

  VertexType vertices[2];
  uint32_t    dataSize;
  void init()
  {
    MESHTYPE::vertices(true); // create vertex array
    MESHTYPE::indices(true);  // create index array
    MESHTYPE::vertexBuffer->bindBufferData(NULL, 2); // create empty buffer    
    typename MESHTYPE::IndexType idx[2];
    idx[0] = 0;
    idx[1] = 1;
    MESHTYPE::indexBuffer->bindBufferData(idx, 2);
    dataSize = sizeof(vertices);
    MESHTYPE::drawMode = GL_LINE_STRIP;
  }

  Line(const VertexType& start, const VertexType& end)
  {
    init();
    update(start, end);
  }
  
  Line()
  {
    init();
  }

  static lost::shared_ptr<Line<MESHTYPE> > create()
  {
    return lost::shared_ptr<Line<MESHTYPE> >(new Line<MESHTYPE>());
  }
  
  static lost::shared_ptr<Line<MESHTYPE> > create(const VertexType& start, const VertexType& end)
  {
    return lost::shared_ptr<Line<MESHTYPE> >(new Line<MESHTYPE>(start, end));
  }
  
  virtual ~Line()
  {
  }
  
  void update(const VertexType& inStart, const VertexType& inEnd)
  {
    vertices[0]=inStart;
    vertices[1] = inEnd;
    MESHTYPE::vertexBuffer->bindBufferSubData(0,dataSize, vertices);
  }
};

typedef Line<Mesh2D> Line2D;
typedef lost::shared_ptr<Line2D> Line2DPtr;
typedef Line<Mesh3D> Line3D;
typedef lost::shared_ptr<Line3D> Line3DPtr;

}
}

#endif
