#ifndef LOST_FONT_MODEL
#define LOST_FONT_MODEL

#include <stdint.h>
#include <boost/shared_ptr.hpp>

namespace lost
{
  
namespace gl{ struct State; }
  
namespace font
{

struct Model
{
  Model();
  virtual ~Model();
  
  void render();
  
  uint32_t vertexCount;
//  boost::shared_array<float>    vertices;
//  boost::shared_array<uint8_t>  indices;
//  boost::shared_ptr<gl::State>  renderState;
  
};

}
}
#endif