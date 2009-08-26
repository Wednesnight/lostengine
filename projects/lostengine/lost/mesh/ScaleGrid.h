#ifndef LOST_MESH_SCALEGRID_H
#define LOST_MESH_SCALEGRID_H

#include "lost/mesh/Mesh.h"

namespace lost
{
namespace mesh
{

/** A ScaleGrid is basically a 2D Quad comprised of a connected 3x3 quad matrix.
 * The corners defined by the left/right/top/bottom parameters are not scaled. Everything else is.
 * The outline of the 3x3 quads will be blown up to match the size of the provided rect.
 * The l/r/t/b measurements should be within the range of the raw pixel data of the texture to
 * achieve meaningful results. Everything else is undefined and will probably look messed up.
 * The specified rect is treated as pixel coordinates in a 2d coordinate system.
 * l,r,t,b are cap sizes in pixels.
 * 0,0 is the bottom left corner.
 */
template<typename MESHTYPE>
struct ScaleGrid : public MESHTYPE
{
  typedef typename MESHTYPE::VertexType VertexType;

  boost::uint32_t numVertices;
  boost::uint32_t numIndices;
  boost::uint32_t numTexCoords;

  ScaleGrid(gl::TexturePtr tex,
            const math::Rect& rect,
            float left,
            float right,
            float top,
            float bottom)
  {
    this->drawMode = GL_TRIANGLES;

    numVertices = 16; // draw it on paper and you'll see it's correct
    boost::uint32_t numQuads = 9; // it's a 3x3 matrix of quads
    boost::uint32_t numTris = numQuads*2; // each quad is drawn with two tris
    numIndices = numTris*3; // currently, each tri is drawn with 3 indices
    numTexCoords = numVertices; // each vertex receives one texcoord

    this->material->textures.push_back(tex);

    // activate the mesh features we need
    this->indices(true);
    this->vertices(true);
    this->texCoords(true);

    // reserve host memory for data preparation
    this->resetIndices(numIndices);
    this->resetVertices(numVertices);
    this->resetTexCoords(numTexCoords);

    // vertices are created in this order:
    // 0  1  2  3
    // 4  5  6  7
    // 8  9  10 11
    // 12 13 14 15
    // 
    // 0,3,12,15 are the corners of the provided rect
    // all others are derived from the l/r/t/b measurement that were provided.
        
    // first, we calculate the vertex coordinates relative to 0,0 since these are the pixel coordinates 
    // we need to calculate the texel coordinates from the texture.
    this->createVertices(math::Vec2(rect.width, rect.height), left, right, top, bottom);
/*    math::Rect zr(0,0,rect.width, rect.height); // zero based rect for the first pass of the calculations
    float h = rect.height;
    float w = rect.width;
    this->vertexData[0] = VertexType(0,h);
    this->vertexData[1] = VertexType(left, h);
    this->vertexData[2] = VertexType(w-right, h);
    this->vertexData[3] = VertexType(w,h);
    
    this->vertexData[4] = VertexType(0, h-top);
    this->vertexData[5] = VertexType(left, h-top);
    this->vertexData[6] = VertexType(w-right, h-top);
    this->vertexData[7] = VertexType(w, h-top);

    this->vertexData[8] = VertexType(0, bottom);
    this->vertexData[9] = VertexType(left, bottom);
    this->vertexData[10] = VertexType(w-right, bottom);
    this->vertexData[11] = VertexType(w, bottom);

    this->vertexData[12] = VertexType(0,0);
    this->vertexData[13] = VertexType(left, 0);
    this->vertexData[14] = VertexType(w-right, 0);
    this->vertexData[15] = VertexType(w,0);*/


    float tw = std::min(tex->dataWidth, tex->width);
    float th = std::min(tex->dataHeight, tex->height);

    this->texCoordData[0] = tex->normalisedCoord(VertexType(0,th));
    this->texCoordData[1] = tex->normalisedCoord(VertexType(left, th));
    this->texCoordData[2] = tex->normalisedCoord(VertexType(tw-right, th));
    this->texCoordData[3] = tex->normalisedCoord(VertexType(tw, th));
    
    this->texCoordData[4] = tex->normalisedCoord(VertexType(0,th-top));
    this->texCoordData[5] = tex->normalisedCoord(VertexType(left,th-top));
    this->texCoordData[6] = tex->normalisedCoord(VertexType(tw-right, th-top));
    this->texCoordData[7] = tex->normalisedCoord(VertexType(tw, th-top));
    
    this->texCoordData[8] = tex->normalisedCoord(VertexType(0,bottom));
    this->texCoordData[9] = tex->normalisedCoord(VertexType(left,bottom));
    this->texCoordData[10] = tex->normalisedCoord(VertexType(tw-right, bottom));
    this->texCoordData[11] = tex->normalisedCoord(VertexType(tw, bottom));
    
    this->texCoordData[12] = tex->normalisedCoord(VertexType(0,0));
    this->texCoordData[13] = tex->normalisedCoord(VertexType(left,0));
    this->texCoordData[14] = tex->normalisedCoord(VertexType(tw-right,0));
    this->texCoordData[15] = tex->normalisedCoord(VertexType(tw, 0));
    
    // and since the input rect defines the geometry and we built it zero based up to here,
    // we need to add the origin of the input rect to all previously calculated vertices
    // to achieve the final geometry
    VertexType origin(rect.bottomLeft()); 
    for(uint32_t i=0; i<numVertices; ++i)
    {
      this->vertexData[i] += origin;
    }

    // now the boring part: create the indices for all the tris
    // this is the pattern:
    // quads from top left to bottom right, scanned horizontally
    // first the top left tri, then the bottom right one
    // top left starts with top left corner of quad,
    // bottom right starts with bottom left corner of the quad
    // all tris counterclockwise
/*    this->indexData[0] = 0;
    this->indexData[1] = 4;
    this->indexData[2] = 1;
    this->indexData[3] = 4;
    this->indexData[4] = 5;
    this->indexData[5] = 1;*/
        
    this->indexData[0] = 0;
    this->indexData[1] = 4;
    this->indexData[2] = 1;
    this->indexData[3] = 4;
    this->indexData[4] = 5;
    this->indexData[5] = 1;
    this->indexData[6] = 1;
    this->indexData[7] = 5;
    this->indexData[8] = 2;
    this->indexData[9] = 5;
    this->indexData[10] = 6;
    this->indexData[11] = 2;
    this->indexData[12] = 2;
    this->indexData[13] = 6;
    this->indexData[14] = 3;
    this->indexData[15] = 6;
    this->indexData[16] = 7;
    this->indexData[17] = 3;
    this->indexData[18] = 4;
    this->indexData[19] = 8;
    this->indexData[20] = 5;
    this->indexData[21] = 8;
    this->indexData[22] = 9;
    this->indexData[23] = 5;
    this->indexData[24] = 5;
    this->indexData[25] = 9;
    this->indexData[26] = 6;
    this->indexData[27] = 9;
    this->indexData[28] = 10;
    this->indexData[29] = 6;
    this->indexData[30] = 6;
    this->indexData[31] = 10;
    this->indexData[32] = 7;
    this->indexData[33] = 10;
    this->indexData[34] = 11;
    this->indexData[35] = 7;
    this->indexData[36] = 8;
    this->indexData[37] = 12;
    this->indexData[38] = 9;
    this->indexData[39] = 12;
    this->indexData[40] = 13;
    this->indexData[41] = 9;
    this->indexData[42] = 9;
    this->indexData[43] = 13;
    this->indexData[44] = 10;
    this->indexData[45] = 13;
    this->indexData[46] = 14;
    this->indexData[47] = 10;
    this->indexData[48] = 10;
    this->indexData[49] = 14;
    this->indexData[50] = 11;
    this->indexData[51] = 14;
    this->indexData[52] = 15;
    this->indexData[53] = 11;

    this->transferVertices();
    this->transferIndices();
    this->transferTexCoords();
  }
  
  void createVertices(const math::Vec2& inSize,
                      float left,
                      float right,
                      float top,
                      float bottom)
  {
    float h = inSize.x;
    float w = inSize.y;
    this->vertexData[0] = VertexType(0,h);
    this->vertexData[1] = VertexType(left, h);
    this->vertexData[2] = VertexType(w-right, h);
    this->vertexData[3] = VertexType(w,h);
    
    this->vertexData[4] = VertexType(0, h-top);
    this->vertexData[5] = VertexType(left, h-top);
    this->vertexData[6] = VertexType(w-right, h-top);
    this->vertexData[7] = VertexType(w, h-top);

    this->vertexData[8] = VertexType(0, bottom);
    this->vertexData[9] = VertexType(left, bottom);
    this->vertexData[10] = VertexType(w-right, bottom);
    this->vertexData[11] = VertexType(w, bottom);

    this->vertexData[12] = VertexType(0,0);
    this->vertexData[13] = VertexType(left, 0);
    this->vertexData[14] = VertexType(w-right, 0);
    this->vertexData[15] = VertexType(w,0);  
  }

  void updateSize(const math::Vec2& inSize,
                      float left,
                      float right,
                      float top,
                      float bottom)
  {
    this->createVertices(inSize, left, right, top, bottom);
    this->transferVertices();
  }

  static lost::shared_ptr<ScaleGrid<MESHTYPE> > create(gl::TexturePtr tex,
                                                       const math::Rect& rect,
                                                       float left,
                                                       float right,
                                                       float top,
                                                       float bottom)
  {
    return lost::shared_ptr<ScaleGrid<MESHTYPE> >(new ScaleGrid<MESHTYPE>(tex, rect, left, right, top, bottom));
  }
};

typedef ScaleGrid<Mesh2D> ScaleGrid2D;
typedef lost::shared_ptr<ScaleGrid2D> ScaleGrid2DPtr;

}
}

#endif