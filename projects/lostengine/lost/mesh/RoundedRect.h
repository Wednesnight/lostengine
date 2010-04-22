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
  virtual ~RoundedRect() {}
  static RoundedRectPtr create(const TextureManagerPtr& inTextureManager, 
       const math::Vec2 inSize, 
       bool filled, 
       float radius, 
       float lineWidth) 
       {
        return RoundedRectPtr(new RoundedRect(inTextureManager, inSize, filled, radius, lineWidth));
       };

  void size(const math::Vec2& newSize); // recalculates vertices, using Vec2 x/y as width/height
  void radius(float r);
  void roundCorners(bool rbl, bool rbr, bool rtl, bool rtr);
  void showSides(bool top, bool bottom, bool left, bool right); // enable/disable drawing of the sides. Disables rounding for adjacent corners.

  void updateVertices();
  void collapseCorners();
  void updateTexture(); // updates the texture, using textureManager, filled flag an corner radii
  void updateIndices();
  void updateTexcoords0(); // for the disc/ring texture
  void updateTexcoords1(); // helper coords that allow multiplication of color gradient onto primary texture
  void updateSides();

  void commonInit(); // called by all constructors

  bool roundBL;
  bool roundBR;
  bool roundTL;
  bool roundTR;
  
  // show which sides of a rectangle? This is probably only useful for non-filled rects
  bool showTop;
  bool showBottom;
  bool showLeft;
  bool showRight;
  
  TextureManagerPtr textureManager;
  bool filled;
  float lineWidth;
  float _radius;
  math::Vec2 _size; // needed for secondary texture coordinate generation

  uint32_t numVertices;
  uint32_t numIndices;
  uint32_t numTexCoords;
};
}
}

#endif