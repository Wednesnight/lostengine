#ifndef LOST_MESH_ROUNDEDRECT_H
#define LOST_MESH_ROUNDEDRECT_H

#include "lost/mesh/Mesh.h"
#include "lost/mesh/forward.h"

namespace lost
{
namespace mesh
{
struct RoundedRect : Mesh
{
  RoundedRect(const TextureManagerPtr& inTextureManager, 
       const math::Vec2 inSize, 
       bool filled, 
       float radius, // all corners have same radius
       float lineWidth); // optional, set to any value if filled
  RoundedRect(const TextureManagerPtr& inTextureManager, 
       const math::Vec2 inSize, 
       bool filled, 
       float topLeftRadius,
       float topRightRadius,
       float bottomLeftRadius,
       float bottomRightRadius,
       float lineWidth); // optional, set to any value if filled
  virtual ~RoundedRect() {}

  void updateSize(const math::Vec2& newSize); // recalculates vertices, using Vec2 x/y as width/height

  void updateTexture(); // updates the texture, using textureManager, filled flag an corner radii
  void createIndices();
  void createTexcoords0(); // for the disc/ring texture
  void createTexcoords1(); // helper coords that allow multiplication of color gradient onto primary texture

  void commonInit(); // called by all constructors
  
  TextureManagerPtr textureManager;
  bool filled;
  float lineWidth;
  float tlr;
  float trr;
  float blr;
  float brr;
  math::Vec2 size; // needed for secondary texture coordinate generation

  uint32_t numVertices;
  uint32_t numIndices;
  uint32_t numTexCoords;
};
}
}

#endif