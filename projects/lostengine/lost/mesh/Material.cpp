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