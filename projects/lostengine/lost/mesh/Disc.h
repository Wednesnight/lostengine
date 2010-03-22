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
 * requesting an updated texture from the TExtureManager if it receives an updateSize call.
 * Scaling the Mesh through its transform matrix will not refresh the texture.
 * The disc will always be placed with it's center at 0,0,0.
 */
struct Disc : Mesh
{
  Disc(const TextureManagerPtr& inTextureManager, float radius);
  virtual ~Disc() {}
  
  void createIndices();
  void createTexCoords();
  void updateSize(float radius);
  void updateTexture(float radius);  
  
  static DiscPtr create(const TextureManagerPtr& inTextureManager, float radius) 
  {
    return DiscPtr(new Disc(inTextureManager, radius));
  }
  
  TextureManagerPtr textureManager;
};
}
}

#endif