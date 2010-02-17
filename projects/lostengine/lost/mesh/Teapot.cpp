#include "lost/mesh/Teapot.h"

namespace lost
{
namespace mesh
{

Teapot::Teapot()
{
  gl::BufferLayout layout;
  layout.add(gl::ET_vec3_f32, gl::UT_vertex, 0);
  layout.add(gl::ET_vec3_f32, gl::UT_normal, 0);
  this->resetBuffers(layout, gl::ET_u32);
  this->drawMode = GL_TRIANGLE_STRIP;

  uint32_t numVertices = 530;
  
}

Teapot::~Teapot()
{
}

TeapotPtr create() {return TeapotPtr(new Teapot); }

}
}
