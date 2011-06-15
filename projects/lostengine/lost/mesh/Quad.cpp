#include "lost/mesh/Quad.h"
#include "lost/gl/gl.h"
#include "lost/gl/HybridIndexBuffer.h"
#include "lost/gl/HybridVertexBuffer.h"
#include <stdexcept>
namespace lost
{
namespace mesh
{

Quad::Quad()
{
  indexBuffer->drawMode = GL_TRIANGLES;
}

// creates a white rectangle with the given size and position.
Quad::Quad(const math::Rect& inRect)
{
  gl::BufferLayout layout;
  layout.add(gl::ET_vec2_f32, gl::UT_position, 0);
  layout.add(gl::ET_vec3_f32, gl::UT_normal, 0);
  this->resetBuffers(layout, gl::ET_u16);
  
  indexBuffer->drawMode = GL_TRIANGLES;
  uint32_t numQuads = 1;
  uint32_t numVertices = numQuads*4;
  uint32_t numIndices = numQuads*6;
  
  this->vertexBuffer->reset(numVertices);
  this->indexBuffer->reset(numIndices);
  
  createIndices(0);
  createVertices(0,inRect);
}

// tries to build a texture from the provided file data and 
// builds a quad with the size of the data the texture was created with,
// flipping the texture coords vertically if required.
// This is essentially a shortcut for creating quads with the same extents as 
// a loaded bitmap. The default Material has a white draw color.
Quad::Quad(common::DataPtr data, bool flip)
{
  gl::BufferLayout layout;
  layout.add(gl::ET_vec2_f32, gl::UT_position, 0);
  layout.add(gl::ET_vec3_f32, gl::UT_normal, 0);
  layout.add(gl::ET_vec2_f32, gl::UT_texcoord0, 0);
  this->resetBuffers(layout, gl::ET_u16);

  indexBuffer->drawMode = GL_TRIANGLES;
  gl::TexturePtr tex(new gl::Texture(data));
  math::Rect rect(0, 0, (float)tex->dataWidth, (float)tex->dataHeight);
  this->material->textures.push_back(tex);
  uint32_t numQuads = 1;
  uint32_t numVertices = numQuads*4;
  uint32_t numIndices = numQuads*6;
  
  this->vertexBuffer->reset(numVertices);
  this->indexBuffer->reset(numIndices);
  
  createIndices(0);
  createVertices(0, rect);
  createTexCoords(0,flip);
}

Quad::Quad(gl::TexturePtr tex, bool flip)
{
  gl::BufferLayout layout;
  layout.add(gl::ET_vec2_f32, gl::UT_position, 0);
  layout.add(gl::ET_vec3_f32, gl::UT_normal, 0);
  layout.add(gl::ET_vec2_f32, gl::UT_texcoord0, 0);
  this->resetBuffers(layout, gl::ET_u16);

  indexBuffer->drawMode = GL_TRIANGLES;
  math::Rect rect(0, 0, (float)tex->dataWidth, (float)tex->dataHeight);
  this->material->textures.push_back(tex);
  uint32_t numQuads = 1;
  uint32_t numVertices = numQuads*4;
  uint32_t numIndices = numQuads*6;

  this->vertexBuffer->reset(numVertices);
  this->indexBuffer->reset(numIndices);

  createIndices(0);
  createVertices(0, rect);
  createTexCoords(0,flip);    
}

// creates a group of independent rects, all inside one mesh
Quad::Quad(const vector<math::Rect>& rects)
{
  gl::BufferLayout layout;
  layout.add(gl::ET_vec2_f32, gl::UT_position, 0);
  layout.add(gl::ET_vec3_f32, gl::UT_normal, 0);
  this->resetBuffers(layout, gl::ET_u16);

  indexBuffer->drawMode = GL_TRIANGLES;
  uint32_t numQuads = rects.size();
  uint32_t numVertices = numQuads*4;
  uint32_t numIndices = numQuads*6;

  this->vertexBuffer->reset(numVertices);
  this->indexBuffer->reset(numIndices);

  for(uint32_t i=0; i<numQuads; ++i)
  {
    createIndices(i);
    createVertices(i,rects[i]);
  }
}

// tries to create rects.size() number of quads, texturing them with tex, using the provided 
// pixelCoords to calculate texture cooridnates.
// rects.size() must be equal to pixelCoords.size() or it will throw.
Quad::Quad(const vector<math::Rect>& rects,
     gl::TexturePtr tex,
     const vector<math::Rect>& pixelCoords,
     bool flip)
{
  this->init(rects, tex, pixelCoords, flip);
}

void Quad::init(const vector<math::Rect>& rects,
          gl::TexturePtr tex,
          const vector<math::Rect>& pixelCoords,
          bool flip)
{
  if(rects.size() != pixelCoords.size())
  {
    throw std::runtime_error("number of rects and pixelCoords must match");
  }
  
  gl::BufferLayout layout;
  layout.add(gl::ET_vec2_f32, gl::UT_position, 0);
  layout.add(gl::ET_vec3_f32, gl::UT_normal, 0);
  layout.add(gl::ET_vec2_f32, gl::UT_texcoord0, 0);
  this->resetBuffers(layout, gl::ET_u16);
  
  indexBuffer->drawMode = GL_TRIANGLES;
  this->material->textures.clear();
  this->material->textures.push_back(tex);
  uint32_t numQuads = rects.size();
  uint32_t numVertices = numQuads*4;
  uint32_t numIndices = numQuads*6;
  
  this->vertexBuffer->reset(numVertices);
  this->indexBuffer->reset(numIndices);
  
  for(uint32_t i=0; i<numQuads; ++i)
  {
    createIndices(i);
    createVertices(i, rects[i]);
    createTexCoords(i, 0, pixelCoords[i], flip);
  }
}

Quad::~Quad() {}

// writes the indices for a given quad into the provided buffer
// two triangles, counterclockwise
void Quad::createIndices(uint32_t quadNum)
{ 
  const uint32_t numIndicesPerQuad = 6;
  uint32_t indexOffset = quadNum*numIndicesPerQuad;
  uint32_t vertexOffset = quadNum*4;
  this->setIndex(indexOffset+0, vertexOffset + 0);
  this->setIndex(indexOffset+1, vertexOffset + 1);
  this->setIndex(indexOffset+2, vertexOffset + 2);
  this->setIndex(indexOffset+3, vertexOffset + 2);
  this->setIndex(indexOffset+4, vertexOffset + 3);
  this->setIndex(indexOffset+5, vertexOffset + 0);    
}

void Quad::createVertices(uint32_t quadNum, const math::Rect& inRect)
{
  uint32_t verticesPerQuad = 4;
  uint32_t offset = quadNum*verticesPerQuad;

  this->setVertex(offset+0, math::Vec2(inRect.x,inRect.y));
  this->setVertex(offset+1, math::Vec2(inRect.x+inRect.width,inRect.y));
  this->setVertex(offset+2, math::Vec2(inRect.x+inRect.width,inRect.y+inRect.height));
  this->setVertex(offset+3, math::Vec2(inRect.x,inRect.y+inRect.height));

  // calculate normals
  math::Vec3 v1(inRect.x,inRect.y,0);
  math::Vec3 v2(inRect.x+inRect.width,inRect.y,0);
  math::Vec3 v3(inRect.x+inRect.width,inRect.y+inRect.height,0);
  math::Vec3 v4(inRect.x,inRect.y+inRect.height,0);
  math::Vec3 normal1 = cross(v2-v1, v3-v2);
  normalise(normal1);
  math::Vec3 normal2 = cross(v4-v3, v1-v4);
  normalise(normal2);
  this->vertexBuffer->set(offset+0, gl::UT_normal, normal1+normal2);
  this->vertexBuffer->set(offset+1, gl::UT_normal, normal1);
  this->vertexBuffer->set(offset+2, gl::UT_normal, normal1+normal2);
  this->vertexBuffer->set(offset+3, gl::UT_normal, normal1);
}
            
void Quad::updateSize(const math::Vec2& size)
{
  math::Rect rect(0, 0, size.width, size.height);
  createVertices(0, rect);
}

// recalculates the texture coordinates for a given quad and texture 0 so the textures data stretechs over the
// whole quad.
void Quad::createTexCoords(uint32_t quadNum,
                     bool flip)
{
  createTexCoords(quadNum, 0, flip);
}

// recalculates the texture coordinates for a given quad and texture so the textures data stretechs over the
// whole quad.
void Quad::createTexCoords(uint32_t quadNum,       // the index of the quad whose texture coordinates should be updated
                     uint32_t texNum,        // the texture to use fo rthe texcoord generation
                     bool flip)                // if flip 0 true, texture coordinates will be flipped vertically
{
  gl::TexturePtr tex = this->material->textures[texNum];
  math::Rect pixelCoords(0, 0, (float)tex->dataWidth, (float)tex->dataHeight);
  createTexCoords(quadNum, texNum, pixelCoords, flip);
}

// recalculates the texture coordinates for a given quad
void Quad::createTexCoords(uint32_t quadNum,       // the index of the quad whose texture coordinates should be updated
                     uint32_t texNum,        // the texture to use fo rthe texcoord generation
                     const math::Rect& pixelRect,   // a rect describing a section of the texture in pixel coordinates
                     bool flip)                // if flip 0 true, texture coordinates will be flipped vertically
{
  // FIXME: since texcoords can have any type, we'd probably need something like 
  // TexCoordType::maxX instead of 1 etc., but it'll work for now
  // i.e. the values might have to be converted from normalised float texture coordinates 
  // to something else like uint8_t etc.
  gl::TexturePtr tex = this->material->textures[texNum];

  math::Vec2 bl = tex->normalisedCoord(pixelRect.bottomLeft());
  math::Vec2 tr = tex->normalisedCoord(math::Vec2(pixelRect.x+pixelRect.width, pixelRect.y+pixelRect.height)/*pixelRect.topRight()*/);
  
  const uint32_t texCoordsPerQuad = 4; 
  uint32_t offset = texCoordsPerQuad*quadNum;
  
  if(flip)
  {
    this->setTexCoord(offset+0, math::Vec2(bl.x,tr.y));
    this->setTexCoord(offset+1, math::Vec2(tr.x,tr.y));
    this->setTexCoord(offset+2, math::Vec2(tr.x,bl.y));
    this->setTexCoord(offset+3, math::Vec2(bl.x,bl.y));
  }
  else
  {
    this->setTexCoord(offset+0, math::Vec2(bl.x,bl.y));
    this->setTexCoord(offset+1, math::Vec2(tr.x,bl.y));
    this->setTexCoord(offset+2, math::Vec2(tr.x,tr.y));
    this->setTexCoord(offset+3, math::Vec2(bl.x,tr.y));
  }
}

}
}
