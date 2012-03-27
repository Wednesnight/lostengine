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

#ifndef LOST_MESH_DISC_H
#define LOST_MESH_DISC_H

#include "lost/mesh/forward.h"
#include "lost/mesh/Mesh.h"

namespace lost
{
namespace mesh
{

/** Creates a Quad with a disc texture obtained fmor the provided TextureManager.
 * The Disc Mesh will attempt to maintain optimal fidelity of the disc texture by 
 * requesting an updated texture from the TextureManager if it receives an updateSize call.
 * Scaling the Mesh through its transform matrix will not refresh the texture.
 * The disc will always be placed with it's center at 0,0,0.
 */
struct Disc : Mesh
{
  Disc(const TextureManagerPtr& inTextureManager, bool filled, float radius, float lineWidth);
  virtual ~Disc() {}
  static DiscPtr create(const TextureManagerPtr& inTextureManager, bool filled, float radius, float lineWidth) 
  {
    return DiscPtr(new Disc(inTextureManager, filled, radius, lineWidth));
  }

  void update(bool filled, float radius, float lineWidth); // call this to resize the Quad, texture will be resized automatically if possible/necessary
  void updateRadius(float newRadius); // sets new diameter, recalculates mesh, uses all other cached values
  void updateLineWidth(float newLineWidth); // sets new lineWidth, recalculates mesh, uses all other cached values

  
  void updateIndices();
  void updateTexCoords0();
  void updateTexCoords1();
  void updateVertices();
  void updateTexture(bool filled, float radius, float lineWidth);  
  
  TextureManagerPtr textureManager; // maintained for texture recreation
  // cached so you don't have to specify all params when calling updateSize
  float filled; 
  float radius;
  float lineWidth;
};
}
}

#endif
