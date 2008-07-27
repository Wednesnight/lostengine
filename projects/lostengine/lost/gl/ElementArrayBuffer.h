#ifndef LOST_GL_ELEMENTARRAYBUFFER_H
#define LOST_GL_ELEMENTARRAYBUFFER_H

#include "lost/gl/Buffer.h"

namespace lost
{
namespace gl
{
template<typename T>
struct ElementArrayBuffer : public Buffer<T>
{
  ElementArrayBuffer()
  {
    target=GL_ELEMENT_ARRAY_BUFFER;
  }
  
  virtual ~ElementArrayBuffer()
  {
  }
};
}
}

#endif