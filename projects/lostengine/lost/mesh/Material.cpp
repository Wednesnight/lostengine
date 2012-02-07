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

#include "lost/mesh/Material.h"

namespace lost
{
namespace mesh
{

Material::Material()
{
  // only initialise scalar values, all others will be set by their respective constructors
  blend = false;
  blendSrc = GL_SRC_ALPHA;
  blendDest = GL_ONE_MINUS_SRC_ALPHA; 
  color = common::whiteColor;
  cull= false;
  cullMode = GL_BACK;
}

Material::~Material()
{
}

MaterialPtr Material::create()
{
  MaterialPtr result;
  result.reset(new Material);
  return result;
}

void Material::blendNormal()
{
  blend = true;
  blendSrc = GL_SRC_ALPHA;
  blendDest = GL_ONE_MINUS_SRC_ALPHA; 
}

void Material::blendPremultiplied()
{
  blend = true;
  blendSrc = GL_ONE;
  blendDest = GL_ONE_MINUS_SRC_ALPHA; 
}

void Material::blendOff()
{
  blend = false;
}

void Material::add(const gl::TexturePtr& tex)
{
  textures.push_back(tex);
}

void Material::setTexture(uint32_t texIndex, const gl::TexturePtr& tex)
{
  if(textures.size() < texIndex+1)
    textures.resize(texIndex+1);
  textures[texIndex] = tex;
}

gl::TexturePtr Material::getTexture(uint32_t idx)
{
  return textures[idx];
}

void Material::limitTextures(uint32_t num) // throws away all textures with index > num
{
  textures.resize(num);
}

MaterialPtr Material::clone()
{
  MaterialPtr result(new Material);
  
  result->textures = textures;
  result->color = color;
  result->blend = blend;
  result->blendSrc = blendSrc;
  result->blendDest = blendDest;
  result->shader = shader; 
  result->uniforms =uniforms;
  result->cull = cull;
  result->cullMode = cullMode;
  
  return result;
}

}
}
