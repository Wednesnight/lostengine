#ifndef LOST_MESH_QUAD_H
#define LOST_MESH_QUAD_H

#include "lost/mesh/Mesh.h"
#include "lost/common/Data.h"
#include "lost/gl/Texture.h"
#include "lost/gl/TextureTile.h"
#include "lost/math/Rect.h"
#include <vector>

namespace lost
{
namespace mesh
{

struct Quad;
typedef lost::shared_ptr<Quad> QuadPtr;
// This class, even though it looks relatively flexible, is meant as a simple 2D Quad replacement.
// it draws a quad with two triangles. Size is derived from a math::Rect. If a texture is provided, 
// the texture coords are calculated and a buffer created for them.
struct Quad : public Mesh
{
  // doesn't create any data or buffers, only a quad with a default drawMode and Material
  Quad();  
  // creates a white rectangle with the given size and position.
  Quad(const math::Rect& inRect);
  
  // tries to build a texture from the provided file data and 
  // builds a quad with the size of the data the texture was created with,
  // flipping the texture coords vertically if required.
  // This is essentially a shortcut for creating quads with the same extents as 
  // a loaded bitmap. The default Material has a white draw color.
  Quad(common::DataPtr data, bool flip=true);  
  Quad(gl::TexturePtr tex, bool flip=true);  
  // creates a group of independent rects, all inside one mesh
  Quad(const std::vector<math::Rect>& rects);
  
  // tries to create rects.size() number of quads, texturing them with tex, using the provided 
  // pixelCoords to calculate texture cooridnates.
  // rects.size() must be equal to pixelCoords.size() or it will throw.
  Quad(const std::vector<math::Rect>& rects,
       gl::TexturePtr tex,
       const std::vector<math::Rect>& pixelCoords,
       bool flip);
  
  void init(const std::vector<math::Rect>& rects,
            gl::TexturePtr tex,
            const std::vector<math::Rect>& pixelCoords,
            bool flip);
  
  static QuadPtr create() { return QuadPtr(new Quad()); }
  static QuadPtr create(const math::Rect& inRect) { return QuadPtr(new Quad(inRect)); }
  static QuadPtr create(const std::vector<math::Rect>& rects) { return QuadPtr(new Quad(rects)); }
  static QuadPtr create(common::DataPtr data, bool flip=true) { return QuadPtr(new Quad(data, flip)); }
  static QuadPtr create(gl::TexturePtr tex, bool flip=true) { return QuadPtr(new Quad(tex, flip)); }
  
  virtual ~Quad();
  
  // writes the indices for a given quad into the provided buffer
  // two triangles, counterclockwise
  void createIndices(boost::uint32_t quadNum);  
  void createVertices(boost::uint32_t quadNum, const math::Rect& inRect);
  void updateSize(const math::Vec2& size);

  // recalculates the texture coordinates for a given quad and texture 0 so the textures data stretechs over the
  // whole quad.
  void createTexCoords(boost::uint32_t quadNum,
                       bool flip=true);

  // recalculates the texture coordinates for a given quad and texture so the textures data stretechs over the
  // whole quad.
  void createTexCoords(boost::uint32_t quadNum,       // the index of the quad whose texture coordinates should be updated
                       boost::uint32_t texNum,        // the texture to use fo rthe texcoord generation
                       bool flip=true);               // if flip 0 true, texture coordinates will be flipped vertically
  
  // recalculates the texture coordinates for a given quad
  void createTexCoords(boost::uint32_t quadNum,       // the index of the quad whose texture coordinates should be updated
                       boost::uint32_t texNum,        // the texture to use fo rthe texcoord generation
                       const math::Rect& pixelRect,   // a rect describing a section of the texture in pixel coordinates
                       bool flip=true);               // if flip 0 true, texture coordinates will be flipped vertically
};
}
}


#endif
