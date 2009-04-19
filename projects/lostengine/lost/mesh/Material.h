#ifndef LOST_MESH_MATERIAL_H
#define LOST_MESH_MATERIAL_H

#include <boost/shared_ptr.hpp>
#include "lost/gl/ShaderProgram.h"
#include "lost/gl/Texture.h"
#include <vector>

namespace lost
{
namespace mesh
{

struct Material;
typedef boost::shared_ptr<Material> MaterialPtr;

struct Material
{
  std::vector<gl::TexturePtr> textures;
  common::Color               color;
  gl::ShaderProgramPtr        shader;
  // FIXME: shaders will probably need dedicated parameter sets that can bes associated with a certin material
};
}
}

#endif