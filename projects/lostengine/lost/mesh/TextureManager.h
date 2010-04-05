#ifndef LOST_MESH_TEXTUREMANAGER_H
#define LOST_MESH_TEXTUREMANAGER_H

#include "lost/gl/forward.h"
#include "lost/mesh/forward.h"
#include <map>
#include <vector>
#include "lost/common/forward.h"

namespace lost
{
namespace mesh
{

struct TextureManager
{
  TextureManager();
  virtual ~TextureManager();
  static TextureManagerPtr create() { return TextureManagerPtr(new TextureManager); };
  
  // use these two functions to retrieve the texture
  gl::TexturePtr arcTexture(float radius, float lineWidth); 
  gl::TexturePtr arcFilledTexture(float radius);
  float addGradient(const common::ColorGradientPtr& inGradient); // returns horizontal texture coordinate of gradient. Gradients always span full vertical range.
  
  // helpers
  void logStats(); 
  void collectGarbage();
    
  gl::TexturePtr createArcTexture(float lineWidth, float radius);
  gl::TexturePtr createArcFilledTexture(float radius);

  typedef std::map<float, gl::TexturePtr> ArcFilledMap;
  ArcFilledMap _arcFilledMap;

  typedef std::pair<float, float> LineWidthRadius;
  typedef std::map<LineWidthRadius, gl::TexturePtr> ArcMap;
  ArcMap _arcMap;
  
  uint32_t _gradientTextureHeight; // must be power of two
  uint32_t gradientTextureWidth; // must be power of two
  uint32_t _maxNumGradients;
  uint32_t _gradientWidth; // in pixels, should be 2n+1 to avoid filter artefacts
  gl::TexturePtr gradientTexture;
  std::vector<common::ColorGradientPtr> _gradients;
  
  void updateGradientTexture();
  
};

}
}

#endif

