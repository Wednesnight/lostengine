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

  Line(const VertexType& start, const VertexType& end)
  {
    this->drawMode = GL_LINES;
  }
  
  Line()
  {
    this->drawMode = GL_LINES;
  }

  virtual ~Line()
  {
  }

  static lost::shared_ptr<Line<MESHTYPE> > create()
  {
    return lost::shared_ptr<Line<MESHTYPE> >(new Line<MESHTYPE>());
  }
  
  static lost::shared_ptr<Line<MESHTYPE> > create(const VertexType& start, const VertexType& end)
  {
    return lost::shared_ptr<Line<MESHTYPE> >(new Line<MESHTYPE>(start, end));
  }
  
};

typedef Line<Mesh2D> Line2D;
typedef lost::shared_ptr<Line2D> Line2DPtr;
typedef Line<Mesh3D> Line3D;
typedef lost::shared_ptr<Line3D> Line3DPtr;

}
}

#endif
