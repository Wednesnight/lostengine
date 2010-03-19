#ifndef LOST_MESH_TEXTUREMANAGER_H
#define LOST_MESH_TEXTUREMANAGER_H

#include "lost/gl/forward.h"
#include "lost/mesh/forward.h"
#include <map>

namespace lost
{
namespace mesh
{

struct TextureManager
{
  TextureManager();
  virtual ~TextureManager();
  
  // textures are cached, reused and always rounded up to the next power-of-two
  gl::TexturePtr ringTexture(float radius, float lineWidth); // in pixels,radius is rounded up, linewidth is used as is
  gl::TexturePtr discTexture(float radius);
  
  void logStats(); // calculates how much memory is probably used on the gfx hardware by the textures known to the TextureManager andlogs them using DOUT
  
  static TextureManagerPtr create() { return TextureManagerPtr(new TextureManager); };
  
  // recreates the disc texture by creating a disc with size sideLength and all mipmaps
  // assumes diameter is a power of two
  void updateDiscTexture(float diameter); 

  
  float _discTextureRadius;
  gl::TexturePtr _discTexture; // theres is only one which is the power-oftwo maximum of all previously used

  struct RadiusLineWidth
  {
    RadiusLineWidth(float r, float lw) : radius(r), lineWidth(lw) {}
    float radius;
    float lineWidth;
  };
  std::map<RadiusLineWidth, gl::TexturePtr> _rlw2texture;
};

}
}

#endif
