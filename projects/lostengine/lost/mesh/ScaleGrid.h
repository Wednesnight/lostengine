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
            float bottom,
            bool flip) // flips texture coordinates vertically if true
  {
    gl::BufferLayout layout;
    layout.add(gl::ET_vec2_f32, gl::UT_vertex, 0);
    layout.add(gl::ET_vec2_f32, gl::UT_texcoord0, 0);
    this->resetBuffers(layout, gl::ET_u32);  
  
    this->drawMode = GL_TRIANGLES;

    numVertices = 16; // draw it on paper and you'll see it's correct
    boost::uint32_t numQuads = 9; // it's a 3x3 matrix of quads
    boost::uint32_t numTris = numQuads*2; // each quad is drawn with two tris
    numIndices = numTris*3; // currently, each tri is drawn with 3 indices
    numTexCoords = numVertices; // each vertex receives one texcoord

    this->_vertexBuffer->reset(numVertices);
    this->_indexBuffer->reset(numIndices);

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

    float tw = std::min(tex->dataWidth, tex->width);
    float th = std::min(tex->dataHeight, tex->height);
  
    if(flip)
    {
      this->setTexCoord(0, tex->normalisedCoord(VertexType(0,0)));
      this->setTexCoord(1, tex->normalisedCoord(VertexType(left, 0)));
      this->setTexCoord(2, tex->normalisedCoord(VertexType(tw-right, 0)));
      this->setTexCoord(3, tex->normalisedCoord(VertexType(tw, 0)));
      
      this->setTexCoord(4, tex->normalisedCoord(VertexType(0,bottom)));
      this->setTexCoord(5, tex->normalisedCoord(VertexType(left,bottom)));
      this->setTexCoord(6, tex->normalisedCoord(VertexType(tw-right,bottom)));
      this->setTexCoord(7, tex->normalisedCoord(VertexType(tw,bottom)));
      
      this->setTexCoord(8, tex->normalisedCoord(VertexType(0,th-top)));
      this->setTexCoord(9, tex->normalisedCoord(VertexType(left,th-top)));
      this->setTexCoord(10, tex->normalisedCoord(VertexType(tw-right, th-top)));
      this->setTexCoord(11, tex->normalisedCoord(VertexType(tw, th-top)));
      
      this->setTexCoord(12, tex->normalisedCoord(VertexType(0,th)));
      this->setTexCoord(13, tex->normalisedCoord(VertexType(left,th)));
      this->setTexCoord(14, tex->normalisedCoord(VertexType(tw-right,th)));
      this->setTexCoord(15, tex->normalisedCoord(VertexType(tw, th)));
    }
    else
    {
      this->setTexCoord(0, tex->normalisedCoord(VertexType(0,th)));
      this->setTexCoord(1, tex->normalisedCoord(VertexType(left, th)));
      this->setTexCoord(2, tex->normalisedCoord(VertexType(tw-right, th)));
      this->setTexCoord(3, tex->normalisedCoord(VertexType(tw, th)));
      
      this->setTexCoord(4, tex->normalisedCoord(VertexType(0,th-top)));
      this->setTexCoord(5, tex->normalisedCoord(VertexType(left,th-top)));
      this->setTexCoord(6, tex->normalisedCoord(VertexType(tw-right, th-top)));
      this->setTexCoord(7, tex->normalisedCoord(VertexType(tw, th-top)));
      
      this->setTexCoord(8, tex->normalisedCoord(VertexType(0,bottom)));
      this->setTexCoord(9, tex->normalisedCoord(VertexType(left,bottom)));
      this->setTexCoord(10, tex->normalisedCoord(VertexType(tw-right, bottom)));
      this->setTexCoord(11, tex->normalisedCoord(VertexType(tw, bottom)));
      
      this->setTexCoord(12, tex->normalisedCoord(VertexType(0,0)));
      this->setTexCoord(13, tex->normalisedCoord(VertexType(left,0)));
      this->setTexCoord(14, tex->normalisedCoord(VertexType(tw-right,0)));
      this->setTexCoord(15, tex->normalisedCoord(VertexType(tw, 0)));
    }
    
    
    // and since the input rect defines the geometry and we built it zero based up to here,
    // we need to add the origin of the input rect to all previously calculated vertices
    // to achieve the final geometry
    VertexType origin(rect.bottomLeft()); 
    for(uint32_t i=0; i<numVertices; ++i)
    {
      this->setVertex(i, this->getVertex(i) + origin);
    }

    // now the boring part: create the indices for all the tris
    // this is the pattern:
    // quads from top left to bottom right, scanned horizontally
    // first the top left tri, then the bottom right one
    // top left starts with top left corner of quad,
    // bottom right starts with bottom left corner of the quad
    // all tris counterclockwise        
    this->setIndex(0, 0);
    this->setIndex(1, 4);
    this->setIndex(2, 1);
    this->setIndex(3, 4);
    this->setIndex(4, 5);
    this->setIndex(5, 1);
    this->setIndex(6, 1);
    this->setIndex(7, 5);
    this->setIndex(8, 2);
    this->setIndex(9, 5);
    this->setIndex(10, 6);
    this->setIndex(11, 2);
    this->setIndex(12, 2);
    this->setIndex(13, 6);
    this->setIndex(14, 3);
    this->setIndex(15, 6);
    this->setIndex(16, 7);
    this->setIndex(17, 3);
    this->setIndex(18, 4);
    this->setIndex(19, 8);
    this->setIndex(20, 5);
    this->setIndex(21, 8);
    this->setIndex(22, 9);
    this->setIndex(23, 5);
    this->setIndex(24, 5);
    this->setIndex(25, 9);
    this->setIndex(26, 6);
    this->setIndex(27, 9);
    this->setIndex(28, 10);
    this->setIndex(29, 6);
    this->setIndex(30, 6);
    this->setIndex(31, 10);
    this->setIndex(32, 7);
    this->setIndex(33, 10);
    this->setIndex(34, 11);
    this->setIndex(35, 7);
    this->setIndex(36, 8);
    this->setIndex(37, 12);
    this->setIndex(38, 9);
    this->setIndex(39, 12);
    this->setIndex(40, 13);
    this->setIndex(41, 9);
    this->setIndex(42, 9);
    this->setIndex(43, 13);
    this->setIndex(44, 10);
    this->setIndex(45, 13);
    this->setIndex(46, 14);
    this->setIndex(47, 10);
    this->setIndex(48, 10);
    this->setIndex(49, 14);
    this->setIndex(50, 11);
    this->setIndex(51, 14);
    this->setIndex(52, 15);
    this->setIndex(53, 11);

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
    float w = inSize.x;
    float h = inSize.y;
    this->setVertex(0, VertexType(0,h));
    this->setVertex(1, VertexType(left, h));
    this->setVertex(2, VertexType(w-right, h));
    this->setVertex(3, VertexType(w,h));
    
    this->setVertex(4, VertexType(0, h-top));
    this->setVertex(5, VertexType(left, h-top));
    this->setVertex(6, VertexType(w-right, h-top));
    this->setVertex(7, VertexType(w, h-top));

    this->setVertex(8, VertexType(0, bottom));
    this->setVertex(9, VertexType(left, bottom));
    this->setVertex(10, VertexType(w-right, bottom));
    this->setVertex(11, VertexType(w, bottom));

    this->setVertex(12, VertexType(0,0));
    this->setVertex(13, VertexType(left, 0));
    this->setVertex(14, VertexType(w-right, 0));
    this->setVertex(15, VertexType(w,0));  
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
                                                       float bottom,
                                                       bool flip)
  {
    return lost::shared_ptr<ScaleGrid<MESHTYPE> >(new ScaleGrid<MESHTYPE>(tex, rect, left, right, top, bottom, flip));
  }
};

typedef ScaleGrid<Mesh2D> ScaleGrid2D;
typedef lost::shared_ptr<ScaleGrid2D> ScaleGrid2DPtr;

}
}

#endif