#ifndef LOST_SG_LINE
#define LOST_SG_LINE

#include "lost/sg/Mesh.h"

namespace lost
{
namespace sg
{

template<typename MESHTYPE>
struct Line : public MESHTYPE
{
  typedef typename MESHTYPE::VertexType VertexType;

  VertexType vertices[2];

  Line(const VertexType& start, const VertexType& end)
  {
    
  }
  
  virtual ~Line()
  {
  }
  
  void start(const VertexType& inStart) {vertices[0] = inStart;}
  VertexType start() { return vertices[0];}
  
};

typedef Line<Mesh2D> Line2D;
typedef Line<Mesh3D> Line3D;

}
}

#endif