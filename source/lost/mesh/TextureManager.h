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

#ifndef LOST_MESH_TEXTUREMANAGER_H
#define LOST_MESH_TEXTUREMANAGER_H

#include "lost/gl/forward.h"
#include "lost/mesh/forward.h"
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
  
  // use these functions to retrieve the texture
  gl::TexturePtr lineTexture(const unsigned int lineWidth); 
  gl::TexturePtr arcTexture(float radius, float lineWidth); 
  gl::TexturePtr arcFilledTexture(float radius);
  float addGradient(const common::ColorGradientPtr& inGradient); // returns horizontal texture coordinate of gradient. Gradients always span full vertical range.
  
  // helpers
  void logStats(); 
  void collectGarbage();
    
  gl::TexturePtr createLineTexture(const unsigned int lineWidth);
  gl::TexturePtr createArcTexture(float lineWidth, float radius);
  gl::TexturePtr createArcFilledTexture(float radius);

  typedef map<float, gl::TexturePtr> ArcFilledMap;
  ArcFilledMap _arcFilledMap;

  typedef pair<float, float> LineWidthRadius;
  typedef map<LineWidthRadius, gl::TexturePtr> ArcMap;
  ArcMap _arcMap;

  typedef map<unsigned int, gl::TexturePtr> LineMap;
  LineMap _lineMap;

  uint32_t _gradientTextureHeight; // must be power of two
  uint32_t gradientTextureWidth; // must be power of two
  uint32_t _maxNumGradients;
  uint32_t _gradientWidth; // in pixels, should be 2n+1 to avoid filter artefacts
  gl::TexturePtr gradientTexture;
  vector<common::ColorGradientPtr> _gradients;
  
  void updateGradientTexture();
  
};

}
}

#endif

