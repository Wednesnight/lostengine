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

#ifndef LOST_MESH_MATERIAL_H
#define LOST_MESH_MATERIAL_H

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
  vector<gl::TexturePtr> textures;
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
