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

  // doesn't create any data or buffers, only a quad with a default drawMode and Material
  Quad()
  {
    init();
  }
  
  // creates a white rectangle with the given size and position.
  Quad(const math::Rect& inRect)
  {
    init();
    boost::uint32_t numQuads = 1;
    boost::uint32_t numVertices = numQuads*4;
    boost::uint32_t numIndices = numQuads*6;
    this->indices(true);
    this->vertices(true);
    this->resetIndices(numIndices);
    this->resetVertices(numVertices);
    updateVertices(inRect);
    createQuadIndices(0);
    this->transferIndices();
  }
  
  // tries to build a texture from the provided file data and 
  // builds a quad with the size of the data the texture was created with,
  // flipping the texture coords vertically if required.
  // This is essentially a shortcut for creating quads with the same extents as 
  // a loaded bitmap. The default Material has a white draw color.
  Quad(resource::FilePtr data, bool flip=true)
  {
    init();
    gl::TexturePtr tex(new gl::Texture(data));
    math::Rect rect(0,0,tex->dataWidth, tex->dataHeight);
    this->material->textures.push_back(tex);
    boost::uint32_t numQuads = 1;
    boost::uint32_t numVertices = numQuads*4;
    boost::uint32_t numIndices = numQuads*6;
    boost::uint32_t numTexCoords = numVertices;
    this->indices(true);
    this->vertices(true);
    this->texCoords(true);
    this->resetIndices(numIndices);
    this->resetVertices(numVertices);
    this->resetTexCoords(numTexCoords);
    createQuadIndices(0);
    this->transferIndices();
    updateVertices(rect);
    updateTexCoords(flip);
  }
  
  Quad(gl::TexturePtr tex, bool flip=true)
  {
    init();
    math::Rect rect(0,0,tex->dataWidth, tex->dataHeight);
    this->material->textures.push_back(tex);
    boost::uint32_t numQuads = 1;
    boost::uint32_t numVertices = numQuads*4;
    boost::uint32_t numIndices = numQuads*6;
    boost::uint32_t numTexCoords = numVertices;
    this->indices(true);
    this->vertices(true);
    this->texCoords(true);
    this->resetIndices(numIndices);
    this->resetVertices(numVertices);
    this->resetTexCoords(numTexCoords);
    createQuadIndices(0);
    this->transferIndices();
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
  void createQuadIndices(boost::uint32_t quadNum)
  { 
    const boost::uint32_t numIndicesPerQuad = 6;
    boost::uint32_t indexOffset = quadNum*numIndicesPerQuad;
    IndexType* idx = this->indexData.get();
    idx[indexOffset+0] = indexOffset + 0;
    idx[indexOffset+1] = indexOffset + 1;
    idx[indexOffset+2] = indexOffset + 2;
    idx[indexOffset+3] = indexOffset + 2;
    idx[indexOffset+4] = indexOffset + 3;
    idx[indexOffset+5] = indexOffset + 0;    
  }
  
  void createVertices(uint32_t quadNum, const math::Rect& inRect)
  {
    float voffsetx = MESHTYPE::OffsetVectorType::x();
    float voffsety = MESHTYPE::OffsetVectorType::y();  
  
    VertexType* vtx = this->vertexData.get();
    boost::uint32_t verticesPerQuad = 4;
    boost::uint32_t offset = quadNum*verticesPerQuad;
  
    vtx[offset+0].x = inRect.x-voffsetx;
    vtx[offset+0].y = inRect.y-voffsety;
    
    vtx[offset+1].x = inRect.maxX()+voffsetx;
    vtx[offset+1].y = inRect.y-voffsety;

    vtx[offset+2].x = inRect.maxX()+voffsetx;
    vtx[offset+2].y = inRect.maxY()+voffsety;

    vtx[offset+3].x = inRect.x-voffsetx;
    vtx[offset+3].y = inRect.maxY()+voffsety;    
  }
    
  void createQuads(std::vector<math::Rect> quadCoords)
  {
    boost::uint32_t nq = quadCoords.size();
    boost::uint32_t nv = nq*4;
    boost::uint32_t ni = nq*6;
    
    // create host data arrays
    this->resetVertices(nv);
    this->resetIndices(ni);
    
    // create hardware buffer objects
    this->vertices(true);
    this->indices(true);

    // create geometry
    for(uint32_t i=0; i<nq; ++i)
    {
      createVertices(i, quadCoords[i]);
    }
    
    // create indices
    for(uint32_t i=0; i<nq; ++i)
    {
      createQuadIndices(i);
    }    
  }
          
  void init()
  {
    // since Quad represents a (non)textured solid quad, drawMode is always GL_TRIANGLES
    MESHTYPE::drawMode = GL_TRIANGLES;
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
  
    VertexType* vertices = this->vertexData.get();
  
    vertices[0].x = inRect.x-voffsetx;
    vertices[0].y = inRect.y-voffsety;
    
    vertices[1].x = inRect.maxX()+voffsetx;
    vertices[1].y = inRect.y-voffsety;

    vertices[2].x = inRect.maxX()+voffsetx;
    vertices[2].y = inRect.maxY()+voffsety;

    vertices[3].x = inRect.x-voffsetx;
    vertices[3].y = inRect.maxY()+voffsety;
    
    this->transferVertices();
  }
  
  // recalculates the texture coordinates from textures[0] if present
  void updateTexCoords(bool flip=true)
  {
    // FIXME: since texcoords can have any type, we'd probably need something like 
    // TexCoordType::maxX instead of 1 etc., but it'll work for now
    // i.e. the values might have to be converted from normalised float texture coordinates 
    // to something else like uint8_t etc.
          
    math::Vec2 bl = this->material->textures[0]->bottomLeftTexCoord();
    math::Vec2 tr = this->material->textures[0]->topRightTexCoord();

    TexCoordType* texcoords = this->texCoordData.get();

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
    this->transferTexCoords();
  }
};

typedef Quad<Mesh2D> Quad2D;
typedef lost::shared_ptr<Quad2D> Quad2DPtr;
typedef Quad<Mesh3D> Quad3D;
typedef lost::shared_ptr<Quad3D> Quad3DPtr;

}
}


#endif
