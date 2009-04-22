#ifndef LOST_SG_MATERIAL_H
#define LOST_SG_MATERIAL_H

#include "lost/gl/Texture.h"
#include "lost/gl/Context.h"

namespace lost
{
namespace sg
{
  
struct Material;
typedef lost::shared_ptr<Material> MaterialPtr;

struct Material
{
  Material() {};
  virtual ~Material() {};
  
  void apply(gl::ContextPtr ctx)
  {
    texture->bind();
  }
  
  gl::TexturePtr texture;
};
}
}

#endif
