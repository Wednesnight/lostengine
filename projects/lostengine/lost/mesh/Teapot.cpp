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

#include "lost/mesh/Teapot.h"

namespace lost
{
namespace mesh
{

Teapot::Teapot()
{
  gl::BufferLayout layout;
  layout.add(gl::ET_vec3_f32, gl::UT_position, 0);
  layout.add(gl::ET_vec3_f32, gl::UT_normal, 0);
  this->resetBuffers(layout, gl::ET_u32);
  indexBuffer->drawMode = GL_TRIANGLE_STRIP;

  uint32_t numVertices = 530;
  
}

Teapot::~Teapot()
{
}

TeapotPtr create() {return TeapotPtr(new Teapot); }

}
}
