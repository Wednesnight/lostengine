#ifndef LOST_MESH_MATERIAL_H
#define LOST_MESH_MATERIAL_H

#include <vector>
#include "lost/mesh/forward.h"
#include "lost/common/Color.h"
#include "lost/gl/gltypes.h"
#include "lost/gl/forward.h"

namespace lost
{
namespace mesh
{

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
  
  Material();
  virtual ~Material();
  static MaterialPtr create();
  void blendNormal();
  void blendPremultiplied();
  void blendOff();
  void add(const gl::TexturePtr& tex);
  void setTexture(uint32_t texIndex, const gl::TexturePtr& tex);
  gl::TexturePtr getTexture(uint32_t idx);
  void limitTextures(uint32_t num); // throws away all textures with index > num
  MaterialPtr clone();
};
}
}

#endif
