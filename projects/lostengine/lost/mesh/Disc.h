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