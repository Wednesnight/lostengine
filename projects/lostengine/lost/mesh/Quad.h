#ifndef LOST_MESH_QUAD_H
#define LOST_MESH_QUAD_H

#include "lost/mesh/Mesh.h"
#include "lost/resource/File.h"
#include "lost/gl/Texture.h"
#include "lost/math/Rect.h"

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
  typedef typename MESHTYPE::TexCoordType TexCoordType;

  VertexType vertices[4];
  TexCoordType texcoords[4];
  uint32_t  vertexBufferSizeBytes;
  uint32_t  texcoordBufferSizeBytes;
  
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
  
  void init()
  {
    numVertices = 4;
    numIndices = 6;
    MESHTYPE::drawMode = GL_TRIANGLES;
    MESHTYPE::vertices(true);
    MESHTYPE::indices(true);
    MESHTYPE::vertexBuffer->bindBufferData(NULL, numVertices); // create empty buffer
    typename MESHTYPE::IndexType idx[numIndices];
    // two triangles, counterclockwise
    idx[0] = 0;
    idx[1] = 1;
    idx[2] = 2;
    idx[3] = 2;
    idx[4] = 3;
    idx[5] = 0;
    MESHTYPE::indexBuffer->bindBufferData(idx, numIndices);
    vertexBufferSizeBytes = sizeof(vertices);
    texcoordBufferSizeBytes = sizeof(texcoords);
    // material defaults to white color without texture
    MESHTYPE::material.reset(new Material);
    MESHTYPE::material->color = common::whiteColor;
  }
      
  void updateVertices(const math::Rect& inRect)
  {
    vertices[0].x = inRect.x-.5;
    vertices[0].y = inRect.y-.5;
    
    vertices[1].x = inRect.maxX()+.5;
    vertices[1].y = inRect.y-.5;

    vertices[2].x = inRect.maxX()+.5;
    vertices[2].y = inRect.maxY()+.5;

    vertices[3].x = inRect.x-.5;
    vertices[3].y = inRect.maxY()+.5;
    
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
      // FIXME: needs texcoord flip switch
      if(flip)
      {
        texcoords[0].x = 0;
        texcoords[0].y = 1;

        texcoords[1].x = 1;
        texcoords[1].y = 1;

        texcoords[2].x = 1;
        texcoords[2].y = 0;

        texcoords[3].x = 0;
        texcoords[3].y = 0;
      }
      else
      {
        texcoords[0].x = 0;
        texcoords[0].y = 0;

        texcoords[1].x = 1;
        texcoords[1].y = 0;

        texcoords[2].x = 1;
        texcoords[2].y = 1;

        texcoords[3].x = 0;
        texcoords[3].y = 1;        
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
