#ifndef LOST_FONT_MODEL
#define LOST_FONT_MODEL

#include <stdint.h>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>

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
  
  uint32_t vertexCount;
  uint32_t indexCount;
  boost::shared_array<float>    vertices;
  boost::shared_array<float>    texcoords;
  boost::shared_array<uint8_t>  indices;
  boost::shared_ptr<gl::State>  renderState;
  boost::shared_ptr<gl::Texture>  texture;
};

}
}
#endif