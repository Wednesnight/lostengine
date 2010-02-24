#include "lost/mesh/Quad.h"

namespace lost
{
namespace mesh
{

Quad::Quad()
{
  this->drawMode = GL_TRIANGLES;
}

// creates a white rectangle with the given size and position.
Quad::Quad(const math::Rect& inRect)
{
  gl::BufferLayout layout;
  layout.add(gl::ET_vec2_f32, gl::UT_position, 0);
  this->resetBuffers(layout, gl::ET_u32);
  
  this->drawMode = GL_TRIANGLES;
  boost::uint32_t numQuads = 1;
  boost::uint32_t numVertices = numQuads*4;
  boost::uint32_t numIndices = numQuads*6;
  
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
  layout.add(gl::ET_vec2_f32, gl::UT_texcoord0, 0);
  this->resetBuffers(layout, gl::ET_u32);

  this->drawMode = GL_TRIANGLES;
  gl::TexturePtr tex(new gl::Texture(data));
  math::Rect rect(0,0,tex->dataWidth, tex->dataHeight);
  this->material->textures.push_back(tex);
  boost::uint32_t numQuads = 1;
  boost::uint32_t numVertices = numQuads*4;
  boost::uint32_t numIndices = numQuads*6;
  
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
  layout.add(gl::ET_vec2_f32, gl::UT_texcoord0, 0);
  this->resetBuffers(layout, gl::ET_u32);

  this->drawMode = GL_TRIANGLES;
  math::Rect rect(0,0,tex->dataWidth, tex->dataHeight);
  this->material->textures.push_back(tex);
  boost::uint32_t numQuads = 1;
  boost::uint32_t numVertices = numQuads*4;
  boost::uint32_t numIndices = numQuads*6;

  this->vertexBuffer->reset(numVertices);
  this->indexBuffer->reset(numIndices);

  createIndices(0);
  createVertices(0, rect);
  createTexCoords(0,flip);    
}

// creates a group of independent rects, all inside one mesh
Quad::Quad(const std::vector<math::Rect>& rects)
{
  gl::BufferLayout layout;
  layout.add(gl::ET_vec2_f32, gl::UT_position, 0);
  this->resetBuffers(layout, gl::ET_u32);

  this->drawMode = GL_TRIANGLES;
  boost::uint32_t numQuads = rects.size();
  boost::uint32_t numVertices = numQuads*4;
  boost::uint32_t numIndices = numQuads*6;

  this->vertexBuffer->reset(numVertices);
  this->indexBuffer->reset(numIndices);

  for(boost::uint32_t i=0; i<numQuads; ++i)
  {
    createIndices(i);
    createVertices(i,rects[i]);
  }
}

// tries to create rects.size() number of quads, texturing them with tex, using the provided 
// pixelCoords to calculate texture cooridnates.
// rects.size() must be equal to pixelCoords.size() or it will throw.
Quad::Quad(const std::vector<math::Rect>& rects,
     gl::TexturePtr tex,
     const std::vector<math::Rect>& pixelCoords,
     bool flip)
{
  this->init(rects, tex, pixelCoords, flip);
}

void Quad::init(const std::vector<math::Rect>& rects,
          gl::TexturePtr tex,
          const std::vector<math::Rect>& pixelCoords,
          bool flip)
{
  if(rects.size() != pixelCoords.size())
  {
    throw std::runtime_error("number of rects and pixelCoords must match");
  }
  
  gl::BufferLayout layout;
  layout.add(gl::ET_vec2_f32, gl::UT_position, 0);
  layout.add(gl::ET_vec2_f32, gl::UT_texcoord0, 0);
  this->resetBuffers(layout, gl::ET_u32);
  
  this->drawMode = GL_TRIANGLES;
  this->material->textures.clear();
  this->material->textures.push_back(tex);
  boost::uint32_t numQuads = rects.size();
  boost::uint32_t numVertices = numQuads*4;
  boost::uint32_t numIndices = numQuads*6;
  
  this->vertexBuffer->reset(numVertices);
  this->indexBuffer->reset(numIndices);
  
  for(boost::uint32_t i=0; i<numQuads; ++i)
  {
    createIndices(i);
    createVertices(i, rects[i]);
    createTexCoords(i, 0, pixelCoords[i], flip);
  }
}

Quad::~Quad() {}

// writes the indices for a given quad into the provided buffer
// two triangles, counterclockwise
void Quad::createIndices(boost::uint32_t quadNum)
{ 
  const boost::uint32_t numIndicesPerQuad = 6;
  boost::uint32_t indexOffset = quadNum*numIndicesPerQuad;
  boost::uint32_t vertexOffset = quadNum*4;
  this->setIndex(indexOffset+0, vertexOffset + 0);
  this->setIndex(indexOffset+1, vertexOffset + 1);
  this->setIndex(indexOffset+2, vertexOffset + 2);
  this->setIndex(indexOffset+3, vertexOffset + 2);
  this->setIndex(indexOffset+4, vertexOffset + 3);
  this->setIndex(indexOffset+5, vertexOffset + 0);    
}

void Quad::createVertices(boost::uint32_t quadNum, const math::Rect& inRect)
{
  boost::uint32_t verticesPerQuad = 4;
  boost::uint32_t offset = quadNum*verticesPerQuad;
    
  this->setVertex(offset+0,math::Vec2(inRect.x,inRect.y));
  this->setVertex(offset+1,math::Vec2(inRect.x+inRect.width,inRect.y));
  this->setVertex(offset+2,math::Vec2(inRect.x+inRect.width,inRect.y+inRect.height));
  this->setVertex(offset+3,math::Vec2(inRect.x,inRect.y+inRect.height));
}
            
void Quad::updateSize(const math::Vec2& size)
{
  math::Rect rect(0, 0, size.width, size.height);
  createVertices(0, rect);
}

// recalculates the texture coordinates for a given quad and texture 0 so the textures data stretechs over the
// whole quad.
void Quad::createTexCoords(boost::uint32_t quadNum,
                     bool flip)
{
  createTexCoords(quadNum, 0, flip);
}

// recalculates the texture coordinates for a given quad and texture so the textures data stretechs over the
// whole quad.
void Quad::createTexCoords(boost::uint32_t quadNum,       // the index of the quad whose texture coordinates should be updated
                     boost::uint32_t texNum,        // the texture to use fo rthe texcoord generation
                     bool flip)                // if flip 0 true, texture coordinates will be flipped vertically
{
  gl::TexturePtr tex = this->material->textures[texNum];
  math::Rect pixelCoords(0,0,tex->dataWidth, tex->dataHeight);
  createTexCoords(quadNum, texNum, pixelCoords, flip);
}

// recalculates the texture coordinates for a given quad
void Quad::createTexCoords(boost::uint32_t quadNum,       // the index of the quad whose texture coordinates should be updated
                     boost::uint32_t texNum,        // the texture to use fo rthe texcoord generation
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
  
  const boost::uint32_t texCoordsPerQuad = 4; 
  boost::uint32_t offset = texCoordsPerQuad*quadNum;
  
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
