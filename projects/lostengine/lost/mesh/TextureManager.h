#ifndef LOST_MESH_TEXTUREMANAGER_H
#define LOST_MESH_TEXTUREMANAGER_H

#include "lost/gl/forward.h"
#include "lost/mesh/forward.h"
#include <map>

namespace lost
{
namespace mesh
{

/** Used to safely create and cache disc and ring textures.
 * Textures are always mimapped, trilinearly filtered, power-of-two.
 * Set maxDiameter to limit the size of the textures. This limit is valid for both disc and ring textures.
 * Ring textures are maintained per unique linewidth, i.e. if you use 100 different lineWidths, you'll end 
 * up with just as many textures, so watch your memory usage.
 */
struct TextureManager
{
  TextureManager();
  virtual ~TextureManager();
  static TextureManagerPtr create() { return TextureManagerPtr(new TextureManager); };
  
  
  // textures are cached, reused and always rounded up to the next power-of-two
  gl::TexturePtr ringTexture(float diameter, float lineWidth); // in pixels,radius is rounded up, linewidth is used as is
  gl::TexturePtr discTexture(float diameter);
  
  void logStats(); // calculates how much memory is probably used on the gfx hardware by the textures known to the TextureManager andlogs them using DOUT
  
  
  // recreates the disc texture by creating a disc with size sideLength and all mipmaps
  // assumes diameter is a power of two
  void updateDiscTexture(float diameter); 
  void updateRingTexture(float diameter, float lineWidth);


  // internal helper functions, you probably won't need these
  void updateTexture(gl::TexturePtr& tex, bool filled, float diameter, float lineWidth);
  float calculateMaxDiameter(float diameter);
  float textureNeedsUpdate(bool filled, float requestedDiameter, float lineWidth);

  float maxDiameter; // used for both disc and ring. set this to some value to prevent textures from growing arbitrarily

  float _discTextureDiameter;
  gl::TexturePtr _discTexture; // theres is only one which is the power-oftwo maximum of all previously used

  std::map<float, float> _lw2ringDiameter; // line width to diameter
  std::map<float, gl::TexturePtr> _lw2ringTexture;

  float _radiusOffset;
  float _centerOffset;
};

}
}

#endif

