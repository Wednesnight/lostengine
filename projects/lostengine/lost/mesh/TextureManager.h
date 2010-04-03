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
  static TextureManagerPtr create() { return TextureManagerPtr(new TextureManager); };
  
  
  gl::TexturePtr arcTexture(float radius, float lineWidth); 
  gl::TexturePtr arcFilledTexture(float radius);
  
  void logStats(); 
  void collectGarbage();
    
  void updateArcFilledTexture(float radius); 


  // internal helper functions, you probably won't need these
  float calculateMaxRadius(float radius);
  float textureNeedsUpdate(float requestedRadius);
  gl::TexturePtr createArcTexture(float lineWidth, float radius);

  float maxRadius; // used for both disc and ring. set this to some value to prevent textures from growing arbitrarily

  float _arcFilledRadius;
  gl::TexturePtr _arcFilledTexture; // theres is only one which is the power-oftwo maximum of all previously used

  typedef std::pair<float, float> LineWidthRadius;
  typedef std::map<LineWidthRadius, gl::TexturePtr> ArcMap;
  ArcMap _arcMap;
  float _centerOffset;
};

}
}

#endif

