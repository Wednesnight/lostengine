#ifndef LOST_MESH_QUAD_H
#define LOST_MESH_QUAD_H

#include "lost/mesh/Mesh.h"
#include "lost/resource/File.h"
#include "lost/gl/Texture.h"
#include "lost/math/Rect.h"
#include <vector>

namespace lost
{
namespace mesh
{

// This class, even though it looks relatively flexible, is meant as a simple 2D Quad replacement.
// it draws a quad with two triangles. Size is derived from a math::Rect. If a texture is provided, 
// the texture coords are calculated and a buffer created for them.
template<typename MESHTYPE>
struct Quad : public MESHTYPE
{
  typedef typename MESHTYPE::VertexType VertexType;
  typedef typename MESHTYPE::IndexType IndexType;
  typedef typename MESHTYPE::TexCoordType TexCoordType;

  VertexType vertices[4];
  TexCoordType texcoords[4];
  uint32_t  vertexBufferSizeBytes;
  uint32_t  texcoordBufferSizeBytes;
  
  uint32_t numQuads;
  uint32_t numVertices;
  uint32_t numIndices;
    
  Quad()
  {
    init();
  }
  
  Quad(const math::Rect& inRect)
  {
    init();
    updateVertices(inRect);
  }
  
  Quad(resource::FilePtr data, bool flip=true)
  {
    init();
    gl::TexturePtr tex(new gl::Texture(data));
    math::Rect rect(0,0,tex->dataWidth, tex->dataHeight);
    MESHTYPE::material->textures.push_back(tex);
    updateVertices(rect);
    updateTexCoords(flip);
  }
  
  Quad(gl::TexturePtr tex, bool flip=true)
  {
    init();
    math::Rect rect(0,0,tex->dataWidth, tex->dataHeight);
    MESHTYPE::material->textures.push_back(tex);
    updateVertices(rect);
    updateTexCoords(flip);
  }
  
  static lost::shared_ptr<Quad<MESHTYPE> > create() { return lost::shared_ptr<Quad<MESHTYPE> >(new Quad<MESHTYPE>()); }
  static lost::shared_ptr<Quad<MESHTYPE> > create(const math::Rect& inRect) { return lost::shared_ptr<Quad<MESHTYPE> >(new Quad<MESHTYPE>(inRect)); }
  static lost::shared_ptr<Quad<MESHTYPE> > create(resource::FilePtr data, bool flip=true) { return lost::shared_ptr<Quad<MESHTYPE> >(new Quad<MESHTYPE>(data, flip)); }
  static lost::shared_ptr<Quad<MESHTYPE> > create(gl::TexturePtr tex, bool flip=true) { return lost::shared_ptr<Quad<MESHTYPE> >(new Quad<MESHTYPE>(tex, flip)); }
  
  virtual ~Quad() {}
  
  // writes the indices for a given quad into the provided buffer
  // two triangles, counterclockwise
  void createQuadIndices(IndexType* idx, boost::uint32_t quadNum)
  { 
    const boost::uint32_t numIndicesPerQuad = 6;
    boost::uint32_t indexOffset = quadNum*numIndicesPerQuad;
    idx[indexOffset+0] = 0;
    idx[indexOffset+1] = 1;
    idx[indexOffset+2] = 2;
    idx[indexOffset+3] = 2;
    idx[indexOffset+4] = 3;
    idx[indexOffset+5] = 0;    
  }
  
  void createVertices(VertexType* vtx, uint32_t quadNum, const math::Rect& inRect)
  {
    float voffsetx = MESHTYPE::OffsetVectorType::x();
    float voffsety = MESHTYPE::OffsetVectorType::y();  
  
    vtx[0].x = inRect.x-voffsetx;
    vtx[0].y = inRect.y-voffsety;
    
    vtx[1].x = inRect.maxX()+voffsetx;
    vtx[1].y = inRect.y-voffsety;

    vtx[2].x = inRect.maxX()+voffsetx;
    vtx[2].y = inRect.maxY()+voffsety;

    vtx[3].x = inRect.x-voffsetx;
    vtx[3].y = inRect.maxY()+voffsety;    
  }
    
  void createQuads(std::vector<math::Rect> quadCoords)
  {
    numQuads = quadCoords.size();
    numVertices = numQuads*4;
    numIndices = numQuads*6;
    // temp buffers for initial creation of geometry
    lost::shared_array<VertexType> vb(new VertexType[numVertices]); 
    lost::shared_array<IndexType> ib(new IndexType[numIndices]);
    MESHTYPE::vertices(true);
    MESHTYPE::indices(true);
    // create geometry
    for(uint32_t i=0; i<numQuads; ++i)
    {
      createVertices(vb.get(), i, quadCoords[i]);
    }
    
    // create indices
    for(uint32_t i=0; i<numQuads; ++i)
    {
      createQuadIndices(ib.get(), i);
    }
    
    // upoad data to buffers
    MESHTYPE::vertexBuffer->bindBufferSubData(0, numVertices*sizeof(VertexType), vb.get());
    
  }
          
  void init()
  {
    numVertices = 4;
    numIndices = 6;
    MESHTYPE::drawMode = GL_TRIANGLES;
    MESHTYPE::vertices(true);
    MESHTYPE::indices(true);
    MESHTYPE::vertexBuffer->bindBufferData(NULL, numVertices); // create empty buffer
    IndexType idx[numIndices];
    createQuadIndices(idx, 0);
    MESHTYPE::indexBuffer->bindBufferData(idx, numIndices);
    vertexBufferSizeBytes = sizeof(vertices);
    texcoordBufferSizeBytes = sizeof(texcoords);
    // material defaults to white color without texture
    MESHTYPE::material.reset(new Material);
    MESHTYPE::material->color = common::whiteColor;
  }

  // FIXME: needs to be specialized to access a certain rectangle inside the buffer
  // OR assume implicit index 0 into mapped vertex buffer
  void updateSize(const math::Vec2& size, bool flip = true)
  {
    math::Rect rect(0, 0, size.width, size.height);
    updateVertices(rect);
    updateTexCoords(flip);
  }

  void updateVertices(const math::Rect& inRect)
  {
    float voffsetx = MESHTYPE::OffsetVectorType::x();
    float voffsety = MESHTYPE::OffsetVectorType::y();  
  
    vertices[0].x = inRect.x-voffsetx;
    vertices[0].y = inRect.y-voffsety;
    
    vertices[1].x = inRect.maxX()+voffsetx;
    vertices[1].y = inRect.y-voffsety;

    vertices[2].x = inRect.maxX()+voffsetx;
    vertices[2].y = inRect.maxY()+voffsety;

    vertices[3].x = inRect.x-voffsetx;
    vertices[3].y = inRect.maxY()+voffsety;
    
    MESHTYPE::vertexBuffer->bindBufferSubData(0, vertexBufferSizeBytes, vertices);
  }
  
  // recalculates the texture coordinates from textures[0] if present
  void updateTexCoords(bool flip=true)
  {
    if(MESHTYPE::material && MESHTYPE::material->textures.size() > 0)
    {
      MESHTYPE::texCoords(true);
      // FIXME: since texcoords can have any type, we'd probably need something like 
      // TexCoordType::maxX instead of 1 etc., but it'll work for now
      // i.e. the values might have to be converted from normalised float texture coordinates 
      // to something else like uint8_t etc.
            
      math::Vec2 bl = MESHTYPE::material->textures[0]->bottomLeftTexCoord();
      math::Vec2 tr = MESHTYPE::material->textures[0]->topRightTexCoord();

      if(flip)
      {
        texcoords[0].x = bl.x;
        texcoords[0].y = tr.y;

        texcoords[1].x = tr.x;
        texcoords[1].y = tr.y;

        texcoords[2].x = tr.x;
        texcoords[2].y = bl.y;

        texcoords[3].x = bl.x;
        texcoords[3].y = bl.y;
      }
      else
      {
        texcoords[0].x = bl.x;
        texcoords[0].y = bl.y;

        texcoords[1].x = tr.x;
        texcoords[1].y = bl.y;

        texcoords[2].x = tr.x;
        texcoords[2].y = bl.y;

        texcoords[3].x = bl.x;
        texcoords[3].y = tr.y;        
      }
      MESHTYPE::texCoordBuffer->bindBufferData(texcoords, numVertices); // don't need to subBufferData
    }
    else
    {
      MESHTYPE::texCoords(false);
    }
  }
};

typedef Quad<Mesh2D> Quad2D;
typedef lost::shared_ptr<Quad2D> Quad2DPtr;
typedef Quad<Mesh3D> Quad3D;
typedef lost::shared_ptr<Quad3D> Quad3DPtr;

}
}


#endif
