/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef LOST_MESH_AXES_H
#define LOST_MESH_AXES_H
/*
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
    u32 numLines = 6;
    // since we use a color array to specify the colorof the lines,
    // and the color should be constant over a half axis, we need
    // toconstruct the lines with two vertices each.
    u32 numVertices = numLines*2;
    u32 numIndices = numVertices;
    u32 numColors = numVertices;
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
    
    this->material->blend = true;
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
*/
#endif
