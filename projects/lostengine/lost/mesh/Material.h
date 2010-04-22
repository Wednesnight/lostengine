#ifndef LOST_MESH_MATERIAL_H
#define LOST_MESH_MATERIAL_H

#include <vector>
#include "lost/common/Color.h"
#include "lost/gl/gltypes.h"
#include "lost/gl/forward.h"

namespace lost
{
namespace mesh
{

struct Material;
typedef lost::shared_ptr<Material> MaterialPtr;

struct Material
{
  std::vector<gl::TexturePtr> textures;
  common::Color               color;
  bool                        blend;
  GLenum                      blendSrc;
  GLenum                      blendDest;
  gl::ShaderProgramPtr        shader; // FIXME: shaders will probably need dedicated parameter sets that can be associated with a certain material
  gl::UniformBlockPtr         uniforms; // optional, will be applied to shader if present
  bool                        cull;
  GLenum                      cullMode;
  
  Material()
  {
    // only initialise scalar values, all others will be set by their respective constructors
    blend = false;
    blendSrc = GL_SRC_ALPHA;
    blendDest = GL_ONE_MINUS_SRC_ALPHA; 
    color = common::whiteColor;
    cull= false;
    cullMode = GL_BACK;
  }
  
  virtual ~Material()
  {
  }
  
  static MaterialPtr create()
  {
    MaterialPtr result;
    result.reset(new Material);
    return result;
  }
  
  void add(const gl::TexturePtr& tex)
  {
    textures.push_back(tex);
  }
  
  void setTexture(uint32_t texIndex, const gl::TexturePtr& tex)
  {
    if(textures.size() < texIndex+1)
      textures.resize(texIndex+1);
    textures[texIndex] = tex;
  }
  
  void limitTextures(uint32_t num) // throws away all textures with index > num
  {
    textures.resize(num);
  }
};
}
}

#endif
