#ifndef LOST_MESH_QUAD_H
#define LOST_MESH_QUAD_H

#include "lost/mesh/Mesh.h"
#include "lost/resource/File.h"
#include "lost/gl/Texture.h"
#include "lost/gl/TextureTile.h"
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
    this->drawMode = GL_TRIANGLES;
  }
  
  // creates a white rectangle with the given size and position.
  Quad(const math::Rect& inRect)
  {
    this->drawMode = GL_TRIANGLES;
    boost::uint32_t numQuads = 1;
    boost::uint32_t numVertices = numQuads*4;
    boost::uint32_t numIndices = numQuads*6;
    this->indices(true);
    this->vertices(true);
    this->resetIndices(numIndices);
    this->resetVertices(numVertices);
    createIndices(0);
    createVertices(0,inRect);
    this->transferIndices();
    this->transferVertices();
  }
  
  // tries to build a texture from the provided file data and 
  // builds a quad with the size of the data the texture was created with,
  // flipping the texture coords vertically if required.
  // This is essentially a shortcut for creating quads with the same extents as 
  // a loaded bitmap. The default Material has a white draw color.
  Quad(resource::FilePtr data, bool flip=true)
  {
    this->drawMode = GL_TRIANGLES;
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
    createIndices(0);
    createVertices(0, rect);
    createTexCoords(0,flip);
    this->transferVertices();
    this->transferIndices();
    this->transferTexCoords();
  }
  
  Quad(gl::TexturePtr tex, bool flip=true)
  {
    this->drawMode = GL_TRIANGLES;
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
    createIndices(0);
    createVertices(0, rect);
    createTexCoords(0,flip);    
    this->transferIndices();
    this->transferVertices();
    this->transferTexCoords();    
  }
  
  // creates a group of independent rects, all inside one mesh
  Quad(const std::vector<math::Rect>& rects)
  {
    this->drawMode = GL_TRIANGLES;
    boost::uint32_t numQuads = rects.size();
    boost::uint32_t numVertices = numQuads*4;
    boost::uint32_t numIndices = numQuads*6;
    this->indices(true);
    this->vertices(true);
    this->resetIndices(numIndices);
    this->resetVertices(numVertices);
    for(boost::uint32_t i=0; i<numQuads; ++i)
    {
      createIndices(i);
      createVertices(i,rects[i]);
    }
    this->transferIndices();
    this->transferVertices();
  }
  
  // tries to create rects.size() number of quads, texturing them with tex, using the provided 
  // pixelCoords to calculate texture cooridnates.
  // rects.size() must be equal to pixelCoords.size() or it will throw.
  Quad(const std::vector<math::Rect>& rects,
       gl::TexturePtr tex,
       const std::vector<math::Rect>& pixelCoords,
       bool flip)
  {
    this->init(rects, tex, pixelCoords, flip);
  }
  
  void init(const std::vector<math::Rect>& rects,
            gl::TexturePtr tex,
            const std::vector<math::Rect>& pixelCoords,
            bool flip)
  {
    if(rects.size() != pixelCoords.size())
    {
      throw std::runtime_error("number of rects and pixelCoords must match");
    }
    this->drawMode = GL_TRIANGLES;
    this->material->textures.clear();
    this->material->textures.push_back(tex);
    boost::uint32_t numQuads = rects.size();
    boost::uint32_t numVertices = numQuads*4;
    boost::uint32_t numIndices = numQuads*6;
    boost::uint32_t numTexCoords = numVertices;
    this->indices(true);
    this->vertices(true);
    this->texCoords(true);
    this->resetIndices(numIndices);
    this->resetVertices(numVertices);
    this->resetTexCoords(numTexCoords);
    for(boost::uint32_t i=0; i<numQuads; ++i)
    {
      createIndices(i);
      createVertices(i, rects[i]);
      createTexCoords(i, 0, pixelCoords[i], flip);
    }
    this->transferIndices();
    this->transferVertices();
    this->transferTexCoords();        
  }
  
  static lost::shared_ptr<Quad<MESHTYPE> > create() { return lost::shared_ptr<Quad<MESHTYPE> >(new Quad<MESHTYPE>()); }
  static lost::shared_ptr<Quad<MESHTYPE> > create(const math::Rect& inRect) { return lost::shared_ptr<Quad<MESHTYPE> >(new Quad<MESHTYPE>(inRect)); }
  static lost::shared_ptr<Quad<MESHTYPE> > create(const std::vector<math::Rect>& rects) { return lost::shared_ptr<Quad<MESHTYPE> >(new Quad<MESHTYPE>(rects)); }
  static lost::shared_ptr<Quad<MESHTYPE> > create(resource::FilePtr data, bool flip=true) { return lost::shared_ptr<Quad<MESHTYPE> >(new Quad<MESHTYPE>(data, flip)); }
  static lost::shared_ptr<Quad<MESHTYPE> > create(gl::TexturePtr tex, bool flip=true) { return lost::shared_ptr<Quad<MESHTYPE> >(new Quad<MESHTYPE>(tex, flip)); }
  
  virtual ~Quad() {}
  
  // writes the indices for a given quad into the provided buffer
  // two triangles, counterclockwise
  void createIndices(boost::uint32_t quadNum)
  { 
    const boost::uint32_t numIndicesPerQuad = 6;
    boost::uint32_t indexOffset = quadNum*numIndicesPerQuad;
    boost::uint32_t vertexOffset = quadNum*4;
    IndexType* idx = this->indexData.get();
    idx[indexOffset+0] = vertexOffset + 0;
    idx[indexOffset+1] = vertexOffset + 1;
    idx[indexOffset+2] = vertexOffset + 2;
    idx[indexOffset+3] = vertexOffset + 2;
    idx[indexOffset+4] = vertexOffset + 3;
    idx[indexOffset+5] = vertexOffset + 0;    
  }
  
  void createVertices(boost::uint32_t quadNum, const math::Rect& inRect)
  {
    VertexType* vtx = this->vertexData.get();
    boost::uint32_t verticesPerQuad = 4;
    boost::uint32_t offset = quadNum*verticesPerQuad;
  
    vtx[offset+0].x = inRect.x;
    vtx[offset+0].y = inRect.y;
    
    vtx[offset+1].x = inRect.x+inRect.width;
    vtx[offset+1].y = inRect.y;

    vtx[offset+2].x = inRect.x+inRect.width;
    vtx[offset+2].y = inRect.y+inRect.height;

    vtx[offset+3].x = inRect.x;
    vtx[offset+3].y = inRect.y+inRect.height;    
  }
              
  void updateSize(const math::Vec2& size, bool flip = true)
  {
    math::Rect rect(0, 0, size.width, size.height);
    createVertices(0, rect);
    this->transferVertices();
  }

  // recalculates the texture coordinates for a given quad and texture 0 so the textures data stretechs over the
  // whole quad.
  void createTexCoords(boost::uint32_t quadNum,
                       bool flip=true)
  {
    createTexCoords(quadNum, 0, flip);
  }

  // recalculates the texture coordinates for a given quad and texture so the textures data stretechs over the
  // whole quad.
  void createTexCoords(boost::uint32_t quadNum,       // the index of the quad whose texture coordinates should be updated
                       boost::uint32_t texNum,        // the texture to use fo rthe texcoord generation
                       bool flip=true)                // if flip 0 true, texture coordinates will be flipped vertically
  {
    gl::TexturePtr tex = this->material->textures[texNum];
    math::Rect pixelCoords(0,0,tex->dataWidth, tex->dataHeight);
    createTexCoords(quadNum, texNum, pixelCoords, flip);
  }
  
  // recalculates the texture coordinates for a given quad
  void createTexCoords(boost::uint32_t quadNum,       // the index of the quad whose texture coordinates should be updated
                       boost::uint32_t texNum,        // the texture to use fo rthe texcoord generation
                       const math::Rect& pixelRect,   // a rect describing a section of the texture in pixel coordinates
                       bool flip=true)                // if flip 0 true, texture coordinates will be flipped vertically
  {
    // FIXME: since texcoords can have any type, we'd probably need something like 
    // TexCoordType::maxX instead of 1 etc., but it'll work for now
    // i.e. the values might have to be converted from normalised float texture coordinates 
    // to something else like uint8_t etc.
    gl::TexturePtr tex = this->material->textures[texNum];

    math::Vec2 bl = tex->normalisedCoord(pixelRect.bottomLeft());
    math::Vec2 tr = tex->normalisedCoord(math::Vec2(pixelRect.x+pixelRect.width, pixelRect.y+pixelRect.height)/*pixelRect.topRight()*/);
    
    TexCoordType* texcoords = this->texCoordData.get();
    const boost::uint32_t texCoordsPerQuad = 4; 
    boost::uint32_t offset = texCoordsPerQuad*quadNum;
    
    if(flip)
    {
      texcoords[offset+0].x = bl.x;
      texcoords[offset+0].y = tr.y;

      texcoords[offset+1].x = tr.x;
      texcoords[offset+1].y = tr.y;

      texcoords[offset+2].x = tr.x;
      texcoords[offset+2].y = bl.y;

      texcoords[offset+3].x = bl.x;
      texcoords[offset+3].y = bl.y;
    }
    else
    {
      texcoords[offset+0].x = bl.x;
      texcoords[offset+0].y = bl.y;

      texcoords[offset+1].x = tr.x;
      texcoords[offset+1].y = bl.y;

      texcoords[offset+2].x = tr.x;
      texcoords[offset+2].y = tr.y;

      texcoords[offset+3].x = bl.x;
      texcoords[offset+3].y = tr.y;        
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
