#ifndef LOST_SG_MATERIAL_H
#define LOST_SG_MATERIAL_H

#include "lost/gl/Texture.h"

namespace lost
{
namespace sg
{
  
struct Material;
typedef boost::shared_ptr<Material> MaterialPtr;

struct Material
{
  Material() {};
  virtual ~Material() {};
  
  gl::TexturePtr texture;
};
}
}

#endif