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
  
  // use these two functions to retrieve the texture
  gl::TexturePtr arcTexture(float radius, float lineWidth); 
  gl::TexturePtr arcFilledTexture(float radius);
  
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
};

}
}

#endif

