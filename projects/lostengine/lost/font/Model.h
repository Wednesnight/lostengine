#ifndef LOST_FONT_MODEL
#define LOST_FONT_MODEL

#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include "lost/math/Vec2.h"

namespace lost
{
  
    namespace gl{ struct State; struct Context; struct Texture; }
  
namespace font
{

struct Model
{
  Model();
  virtual ~Model();
  
  void render(boost::shared_ptr<gl::Context> context);
  
  boost::uint32_t vertexCount;
  boost::uint32_t indexCount;
  boost::shared_array<float>    vertices;
  boost::shared_array<float>    texcoords;
  boost::shared_array<boost::uint8_t>  indices;
  boost::shared_ptr<gl::State>  renderState;
  boost::shared_ptr<gl::Texture>  texture;
  lost::math::Vec2              size; // width and height in pixel of the rendered string. baseline is at y=0
};

}
}
#endif