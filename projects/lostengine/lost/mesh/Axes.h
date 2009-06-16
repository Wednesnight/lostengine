#ifndef LOST_MESH_AXES_H
#define LOST_MESH_AXES_H

#include "lost/mesh/Mesh.h"
#include "lost/math/Axes.h"

namespace lost
{
namespace mesh
{

template<typename MESHTYPE>
struct Axes : public MESHTYPE
{
  Axes()
  {
    this->drawMode = GL_LINES;
    boost::uint32_t numLines = 6;
    boost::uint32_t numVertices = numLines*2;
    boost::uint32_t numIndices = numVertices;
    boost::uint32_t numColors = numVertices;
    this->indices(true);
    this->vertices(true);
    this->colors(true);
    
    // positive 
    
    this->transferVertices();
    this->transferIndices();
    this->transferColors();
  }
  
};

typedef Axes<Mesh3D> Axes3D;
typedef lost::shared_ptr<Axes3D> Axes3DPtr;
  
}
}

#endif