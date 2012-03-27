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

#ifndef LOST_MESH_AABB_H
#define LOST_MESH_AABB_H
/*
#include "lost/mesh/Mesh.h"
#include "lost/math/AABB.h"

namespace lost
{
namespace mesh
{

template<typename MESHTYPE>
struct AABB : MESHTYPE
{
  typedef typename MESHTYPE::VertexType VertexType;

  AABB(const math::AABB& inAABB)
  {
    this->drawMode = GL_LINES;
    u32 numLines = 12;
    u32 numVertices = 8;
    u32 numIndices = numLines*2;
    
    this->indices(true);
    this->vertices(true);
    this->resetIndices(numIndices);
    this->resetVertices(numVertices);
    
    // vertices
    // start at the bottom, counter clockwise
    this->vertexData[0] = VertexType(inAABB.origin.x, inAABB.origin.y, inAABB.origin.z); // front bottom left
    this->vertexData[1] = VertexType(inAABB.origin.x+inAABB.size.x, inAABB.origin.y, inAABB.origin.z); // front bottom right
    this->vertexData[2] = VertexType(inAABB.origin.x+inAABB.size.x, inAABB.origin.y, inAABB.origin.z+inAABB.size.z); // back bottom right
    this->vertexData[3] = VertexType(inAABB.origin.x, inAABB.origin.y, inAABB.origin.z+inAABB.size.z); // back bottom left

    this->vertexData[4] = VertexType(inAABB.origin.x, inAABB.origin.y+inAABB.size.y, inAABB.origin.z); // front top left
    this->vertexData[5] = VertexType(inAABB.origin.x+inAABB.size.x, inAABB.origin.y+inAABB.size.y, inAABB.origin.z); // front top right
    this->vertexData[6] = VertexType(inAABB.origin.x+inAABB.size.x, inAABB.origin.y+inAABB.size.y, inAABB.origin.z+inAABB.size.z); // back top right
    this->vertexData[7] = VertexType(inAABB.origin.x, inAABB.origin.y+inAABB.size.y, inAABB.origin.z+inAABB.size.z); // back top left

    // indices
    // bottom 
    this->indexData[0] = 0;
    this->indexData[1] = 1;
    this->indexData[2] = 1;
    this->indexData[3] = 2;
    this->indexData[4] = 2;
    this->indexData[5] = 3;
    this->indexData[6] = 3;
    this->indexData[7] = 0;
    // mid section
    this->indexData[8] = 0;
    this->indexData[9] = 4;
    this->indexData[10] = 1;
    this->indexData[11] = 5;
    this->indexData[12] = 2;
    this->indexData[13] = 6;
    this->indexData[14] = 3;
    this->indexData[15] = 7;
    // top
    this->indexData[16] = 4;
    this->indexData[17] = 5;
    this->indexData[18] = 5;
    this->indexData[19] = 6;
    this->indexData[20] = 6;
    this->indexData[21] = 7;
    this->indexData[22] = 7;
    this->indexData[23] = 4;

    this->transferVertices();
    this->transferIndices();    
  }

  static lost::shared_ptr<AABB<MESHTYPE> > create(const math::AABB& inAABB)
  {
    return lost::shared_ptr<AABB<MESHTYPE> >(new AABB(inAABB));
  }

};

typedef AABB<Mesh3D> AABB3D;
typedef lost::shared_ptr<AABB3D> AABB3DPtr;


}
}
*/
#endif
