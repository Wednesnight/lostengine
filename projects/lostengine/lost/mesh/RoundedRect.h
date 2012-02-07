/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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
  void lineWidth(float lw);
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
  float _lineWidth;
  float _radius;
  math::Vec2 _size; // needed for secondary texture coordinate generation

  uint32_t numVertices;
  uint32_t numIndices;
  uint32_t numTexCoords;
};
}
}

#endif
