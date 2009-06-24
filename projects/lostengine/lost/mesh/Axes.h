#ifndef LOST_MESH_AXES_H
#define LOST_MESH_AXES_H

#include "lost/mesh/Mesh.h"

namespace lost
{
namespace mesh
{


// length of the axes is currently hard coded to 1
// origin is always 0,0,0, use the transform to change that
template<typename MESHTYPE>
struct Axes : public MESHTYPE
{
  typedef typename MESHTYPE::VertexType VertexType;

  Axes()
  {
    float len = 1;
    this->drawMode = GL_LINES;
    boost::uint32_t numLines = 6;
    // since we use a color array to specify the colorof the lines,
    // and the color should be constant over a half axis, we need
    // toconstruct the lines with two vertices each.
    boost::uint32_t numVertices = numLines*2;
    boost::uint32_t numIndices = numVertices;
    boost::uint32_t numColors = numVertices;
    this->indices(true);
    this->vertices(true);
    this->colors(true);
    this->resetIndices(numIndices);
    this->resetVertices(numVertices);
    this->resetColors(numColors);
    
    float a = .2f;
    
    // X red positive
    this->vertexData[0] = VertexType(0,0,0);
    this->colorData[0] = common::Color(1,0,0,1);
    this->indexData[0] = 0;
    this->vertexData[1] = VertexType(len,0,0);
    this->colorData[1] = common::Color(1,0,0,1);
    this->indexData[1] = 1;

    // X red negative alpha
    this->vertexData[2] = VertexType(0,0,0);
    this->colorData[2] = common::Color(1,0,0,a);
    this->indexData[2] = 2;
    this->vertexData[3] = VertexType(-len,0,0);
    this->colorData[3] = common::Color(1,0,0,a);
    this->indexData[3] = 3;
    
    // Y green positive
    this->vertexData[4] = VertexType(0,0,0);
    this->colorData[4] = common::Color(0,1,0,1);
    this->indexData[4] = 4;
    this->vertexData[5] = VertexType(0,len,0);
    this->colorData[5] = common::Color(0,1,0,1);
    this->indexData[5] = 5;

    // Y green negative
    this->vertexData[6] = VertexType(0,0,0);
    this->colorData[6] = common::Color(0,1,0,a);
    this->indexData[6] = 6;
    this->vertexData[7] = VertexType(0,-len,0);
    this->colorData[7] = common::Color(0,1,0,a);
    this->indexData[7] = 7;

    // Z blue positive
    this->vertexData[8] = VertexType(0,0,0);
    this->colorData[8] = common::Color(0,0,1,1);
    this->indexData[8] = 8;
    this->vertexData[9] = VertexType(0,0,len);
    this->colorData[9] = common::Color(0,0,1,1);
    this->indexData[9] = 9;
    
    // Z blue negative
    this->vertexData[10] = VertexType(0,0,0);
    this->colorData[10] = common::Color(0,0,1,a);
    this->indexData[10] = 10;
    this->vertexData[11] = VertexType(0,0,-len);
    this->colorData[11] = common::Color(0,0,1,a);
    this->indexData[11] = 11;
    
    this->transferVertices();
    this->transferIndices();
    this->transferColors();
  }
  
  static lost::shared_ptr<Axes<MESHTYPE> > create()
  {
    return lost::shared_ptr<Axes<MESHTYPE> >(new Axes<MESHTYPE>);
  }
  
};

typedef Axes<Mesh3D> Axes3D;
typedef lost::shared_ptr<Axes3D> Axes3DPtr;
  
}
}

#endif