#ifndef LOST_MESH_LINE
#define LOST_MESH_LINE
/*
#include "lost/mesh/Mesh.h"

namespace lost
{
namespace mesh
{

template<typename MESHTYPE>
struct Line : public MESHTYPE
{
  typedef typename MESHTYPE::VertexType VertexType;
  typedef typename MESHTYPE::IndexType IndexType;

  Line(const VertexType& start, const VertexType& end)
  {
    this->drawMode = GL_LINES;
    boost::uint32_t numVertices = 2;
    boost::uint32_t numIndices = numVertices;
    this->vertices(true);
    this->indices(true);
    this->resetVertices(numVertices);
    this->resetIndices(numIndices);
    // FIXME: something is still off with these fucking fragemnt center offsets
    this->vertexData[0] = start; //+math::Vec2(.5f, .5f);
    this->vertexData[1] = end; //+math::Vec2(.5f, .5f);
    this->indexData[0] = 0;
    this->indexData[1] = 1;
    this->transferVertices();
    this->transferIndices();
  }
  
  Line(const std::vector<VertexType>& v)
  {
    this->drawMode = GL_LINE_STRIP;
    boost::uint32_t numVertices = v.size();
    boost::uint32_t numIndices = numVertices;
    this->vertices(true);
    this->indices(true);
    this->resetVertices(numVertices);
    this->resetIndices(numIndices);
    for(boost::uint32_t i=0; i<numVertices; ++i)
    {
      this->vertexData[i] = v[i];
      this->indexData[i] = i;
    }
    this->transferVertices();
    this->transferIndices();    
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

  static lost::shared_ptr<Line<MESHTYPE> > create(const std::vector<VertexType>& v)
  {
    return lost::shared_ptr<Line<MESHTYPE> >(new Line<MESHTYPE>(v));
  }
  
};

typedef Line<Mesh2D> Line2D;
typedef lost::shared_ptr<Line2D> Line2DPtr;
typedef Line<Mesh3D> Line3D;
typedef lost::shared_ptr<Line3D> Line3DPtr;

}
}
*/
#endif
